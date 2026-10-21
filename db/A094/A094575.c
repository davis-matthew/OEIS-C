/*
 * A094575  Numbers k with property that binomial(2k, k) / (k-1) is an
 *          integer.
 * https://oeis.org/A094575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k ≥ 2 such that binomial(2k,k)/(k-1) is
 *            an integer. The sequence is computed by testing each k in order
 *            and keeping those that satisfy the divisibility condition using
 *            exponent arithmetic
 * Range:     u8 n <= 33, u16 n <= 7604, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is built lazily:  once a(n) is requested we generate terms
    until we have the nth one.  Results are cached so that repeated calls
    are fast.  The algorithm uses only integer arithmetic, so it works
    for all supported integer sizes. */

#include <stdlib.h>

/*  Exponent of prime p in n!  (Legendre's formula)  */
static inline idx_t exp_in_factorial(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/*  Return true if binomial(2k,k) is divisible by (k-1)  */
static int good_k(idx_t k)
{
    idx_t m = k - 1;
    if (m <= 1)                /* k = 2 => (k-1)=1 divides everything   */
        return 1;
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            idx_t e_bin =
                exp_in_factorial(2 * k, p) - 2 * exp_in_factorial(k, p);
            if (e_bin < cnt)
                return 0;
        }
    }
    if (temp > 1) {          /* temp is a prime factor > sqrt(m)   */
        idx_t p = temp;
        idx_t cnt = 1;
        idx_t e_bin =
            exp_in_factorial(2 * k, p) - 2 * exp_in_factorial(k, p);
        if (e_bin < cnt)
            return 0;
    }
    return 1;
}

/*  Cache of computed terms  */
static seq_t *a_cache = NULL;
static size_t a_len  = 0;   /* number of terms stored           */
static size_t a_cap  = 0;   /* allocated capacity                */
static idx_t   last_k_checked = 1;   /* the largest k processed so far */

/*  Ensure that a_cache contains at least n terms  */
static void ensure_terms(idx_t n)
{
    while (a_len < n) {
        /* find next suitable k */
        do {
            ++last_k_checked;
        } while (!good_k(last_k_checked));

        /* store term */
        if (a_len == a_cap) {          /* grow array if needed */
            size_t new_cap = a_cap ? a_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();         /* allocation failed */
            a_cache = tmp;
            a_cap = new_cap;
        }
        a_cache[a_len++] = (seq_t)last_k_checked;
    }
}

/*  Public sequence function  */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; n must be ≥1 by specification */
        return (seq_t)0;
    ensure_terms(n);
    return a_cache[n - 1];
}
