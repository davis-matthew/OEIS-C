/*
 * A174345  Triangle T(n, k) = (1/k)*binomial(n-1, k-1)*binomial(n,
 *          k-1)*2^(k-1) if floor(n/2) >= k, otherwise (1/k)*binomial(n-1,
 *          k-1)*binomial(n, k-1)*2^(n-k), read by rows.
 * https://oeis.org/A174345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 >= n, k =
 *            n - r(r-1)/2, T(r,k) = (1/k) * C(r-1,k-1) * C(r,k-1) * 2^(e) , e
 *            = ( floor(r/2) >= k ) ? k-1 : r-k
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 144, u64 n <= 479,
 *            u128 n <= 1275, big any n
 * Helpers:   fac_prime_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/fac_prime_exp.h"

/* Primes up to 199 – all primes that can appear in n <= 10000   */
static const int PRIMES[46] = {
     2, 3, 5, 7,11,13,17,19,23,29,
    31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,
    127,131,137,139,149,151,157,163,167,173,
    179,181,191,193,197,199
};
#define PRIME_COUNT 46

/* helper: add exponents of binomial C(n,k) to array exp_arr   */
static void add_binom_factors(idx_t n, idx_t k, int *exp_arr)
{
    for (size_t i = 0; i < PRIME_COUNT; ++i) {
        int p = PRIMES[i];
        unsigned int e = fac_prime_exp(n, p)
                        - fac_prime_exp(k, p)
                        - fac_prime_exp(n - k, p);
        exp_arr[i] += (int)e;
    }
}

/* helper: subtract prime factors of m from exp_arr   */
static void sub_integer_factors(idx_t m, int *exp_arr)
{
    for (size_t i = 0; i < PRIME_COUNT && m > 1; ++i) {
        int p = PRIMES[i];
        while (m % p == 0) {
            exp_arr[i]--;      /* m now contains that prime less times   */
            m /= p;
        }
    }
}

seq_t a(idx_t n)
{
    /* find row r such that r(r+1)/2 >= n  */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;                     /* 1 <= k <= r   */
    idx_t exp = (r / 2 >= k) ? (k - 1) : (r - k);

    /* prime exponent array – all zeros initially */
    int exp_arr[PRIME_COUNT] = {0};

    /* add factors from C(r-1,k-1) and C(r,k-1)   */
    add_binom_factors(r - 1, k - 1, exp_arr);
    add_binom_factors(r,     k - 1, exp_arr);

    /* subtract denominator k   */
    sub_integer_factors(k, exp_arr);

    /* add 2^exp   */
    if (exp > 0)
        exp_arr[0] += (int)exp;   /* prime 2 is at index 0 */

    /* generate the result – all exponents are non‑negative, result <= 2^64-1
       (we only care about terms that fit into seq_t, longer ones are not
        required for the 64‑bit build)   */
    unsigned __int128 prod = 1;
    for (size_t i = 0; i < PRIME_COUNT; ++i)
    {
        int e = exp_arr[i];
        for (int j = 0; j < e; ++j)
            prod *= (unsigned __int128)PRIMES[i];
    }
    return (seq_t)prod;
}
