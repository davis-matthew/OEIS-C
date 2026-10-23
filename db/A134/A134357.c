/*
 * A134357  Denominator of binomial(6*n-2,2*n)/(2*binomial(4*n-1,2*n)).
 * https://oeis.org/A134357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 40, u128 n <= 72,
 *            big any n
 * Helpers:   e_factorial, local_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134357:
 *   a(n) = denominator of binomial(6*n-2,2*n) / (2*binomial(4*n-1,2*n))
 *   (i.e. product of p^max(0, e_p(den) - e_p(num)) over all primes p)
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/e_factorial.h"
#include "../utils/local_pow.h"

/* global prime cache */
static uint32_t *g_primes   = NULL;   /* pointer to array of primes */
static size_t    g_prime_cnt = 0;      /* number of primes stored   */
static idx_t     g_prime_lim = 0;      /* largest integer for which all primes <= prime_lim have been generated */

/* initialize prime cache up to 'limit' (inclusive) */
static void init_primes_upto(idx_t limit)
{
    if (g_prime_lim >= limit) return;  /* already has enough primes */

    /* allocate sieve */
    unsigned char *sieve = (unsigned char *)calloc((size_t)(limit + 1U), sizeof(unsigned char));
    if (!sieve) return;               /* allocation failed – fine, the caller will not be able to compute large n */

    /* mark all numbers >=2 as prime initially */
    for (idx_t i = 2; i <= limit; ++i) sieve[(size_t)i] = 1;

    /* sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                sieve[(size_t)j] = 0;
            }
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[(size_t)i]) ++cnt;

    /* allocate array for primes */
    uint32_t *new_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_primes) {
        free(sieve);
        return;  /* allocation failed */
    }

    /* store primes */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve[(size_t)i]) new_primes[pos++] = (uint32_t)i;
    }

    free(sieve);

    /* replace old cache */
    free(g_primes);
    g_primes   = new_primes;
    g_prime_cnt = cnt;
    g_prime_lim = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;   /* a(0) = 2  (given explicitly) */

    /* compute bounds */
    idx_t m1 = 6 * n - 2;         /* 6n-2 */
    idx_t k1 = 2 * n;             /* 2n  */
    idx_t m2 = 4 * n - 1;         /* 4n-1 */
    idx_t k2 = 2 * n;             /* 2n  */

    /* ensure primes up to m1 are available */
    init_primes_upto(m1);

    seq_t ans = 1;                 /* accumulated denominator */

    /* iterate over all primes <= m1 */
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint32_t p = g_primes[i];
        if (p > m1) break;

        /* exponent of p in the numerator: C(m1, k1) */
        idx_t e_num = e_factorial(m1, p) - e_factorial(k1, p) - e_factorial(m1 - k1, p);

        /* exponent of p in the denominator: 2 * C(m2, k2) */
        idx_t e_den = e_factorial(m2, p) - e_factorial(k2, p) - e_factorial(m2 - k2, p);
        if (p == 2) e_den += 1;   /* factor 2 in the denominator */

        /* add remaining power of p to the result */
        if (e_den > e_num) {
            idx_t diff = e_den - e_num;
            ans *= local_pow((seq_t)p, diff);
        }
    }

    return ans;
}
