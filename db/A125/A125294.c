/*
 * A125294  Numerator of (Sum_{k=1..n} k^2) / (Product_{k=1..n} k^2).
 * https://oeis.org/A125294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(n*(n+1)*(2*n+1)/(6*(n!)^2))
 * Range:     u8 n <= 17, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(1) = 1, a(2) = 5, a(3) = 7, a(4) = 5, a(5) = 11, ... */

/* This implementation uses prime factorisation of the numerator
 * and the denominator.  The remaining prime powers in the numerator
 * after cancelling give the desired value.  It works for any size
 * of seq_t that supports the normal arithmetic operators. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache of primes up to the largest value requested */
static uint64_t *prime_list  = NULL;
static size_t   prime_cnt    = 0;
static size_t   prime_limit  = 0;   /* maximum n for which primes are computed */

/* ensure that prime_list contains all primes <= max (inclusive)      */
static void
ensure_primes(size_t max)
{
    if (max <= prime_limit) return;          /* already computed */

    /* simple sieve of Eratosthenes */
    size_t n = max + 1;
    bool *is_composite = (bool *)calloc(n, sizeof(bool));
    if (!is_composite) return;               /* out of memory – unlikely for small n */
    for (size_t i = 2; i * i < n; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j < n; j += i) {
                is_composite[j] = true;
            }
        }
    }

    /* free previous list and allocate new one */
    free(prime_list);
    prime_list = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!prime_list) {                      /* out of memory – unlikely */
        free(is_composite);
        return;
    }

    prime_cnt = 0;
    for (size_t i = 2; i < n; ++i) {
        if (!is_composite[i]) {
            prime_list[prime_cnt++] = i;
        }
    }
    prime_limit = max;
    free(is_composite);
}

/* exponent of p in integer val (val > 0) */
static inline size_t
exp_in_int(idx_t val, uint64_t p)
{
    size_t cnt = 0;
    while (val % p == 0) {
        val /= p;
        ++cnt;
    }
    return cnt;
}

/* exponent of p in n!  (Legendre's formula) */
static inline size_t
exp_in_factorial(idx_t n, uint64_t p)
{
    size_t cnt = 0;
    while (n) {
        n /= p;
        cnt += n;
    }
    return cnt;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* offset starts at 1 – n is guaranteed to be >= 1 */
    /* base case: n == 1 gives result 1 */
    if (n == 1) return (seq_t)1;

    /* compute upper bound for primes */
    idx_t two_n_plus_one = (n << 1) + 1;
    ensure_primes((size_t)two_n_plus_one);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > (uint64_t)two_n_plus_one) break;

        /* exponent of p in numerator N = n*(n+1)*(2n+1) */
        size_t eN = exp_in_int(n,          p)
                  + exp_in_int(n + 1,      p)
                  + exp_in_int(two_n_plus_one, p);

        /* exponent of p in denominator D = 6 * (n!)^2 */
        size_t eD = 2 * exp_in_factorial(n, p);
        if (p == 2) ++eD;   /* extra factor from 6 = 2 * 3 */
        if (p == 3) ++eD;

        /* remaining exponent after cancelling */
        size_t eR = (eN > eD) ? (eN - eD) : 0;
        if (eR == 0) continue;

        /* multiply result by p^eR */
        seq_t term = (seq_t)1;
        for (size_t j = 0; j < eR; ++j)
            term *= (seq_t)p;
        result *= term;
    }

    return result;
}
