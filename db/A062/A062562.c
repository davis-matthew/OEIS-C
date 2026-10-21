/*
 * A062562  a(n) = Sum_{k=1..n} mu(k)*sigma(k).
 * https://oeis.org/A062562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} mu(k)*sigma(k) (mu = Möbius, sigma = sum of
 *            divisors)
 * Range:     i8 n <= 38, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static size_t max_n = 0;          /* largest n for which we have computed */
static int8_t  *mu      = NULL;   /* Möbius values, signed */
static uint64_t *sigma  = NULL;   /* sum‑of‑divisors values */
static seq_t *prefix   = NULL;    /* partial sums a(k) */

/* Expand the caches to hold values up to 'limit' (inclusive) and recompute everything. */
static void expand_and_compute(size_t limit) {
    size_t i, j;
    /* Resize arrays */
    if (mu      == NULL) mu      = (int8_t *)malloc((limit + 1) * sizeof(int8_t));
    else          mu      = (int8_t *)realloc(mu,      (limit + 1) * sizeof(int8_t));
    if (sigma   == NULL) sigma   = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    else          sigma   = (uint64_t *)realloc(sigma,   (limit + 1) * sizeof(uint64_t));
    if (prefix  == NULL) prefix  = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    else          prefix  = (seq_t *)realloc(prefix,  (limit + 1) * sizeof(seq_t));

    /* --- Compute Möbius via linear sieve --- */
    for (i = 0; i <= limit; ++i) mu[i] = 0;
    {
        static const size_t MAXP = 100000;          /* sufficient for n <= limit <= 1e5 */
        uint32_t primes[MAXP];
        size_t primes_cnt = 0;
        uint8_t *is_comp   = (uint8_t *)calloc(limit + 1, 1);
        mu[1] = 1;
        for (i = 2; i <= limit; ++i) {
            if (!is_comp[i]) {
                primes[primes_cnt++] = (uint32_t)i;
                mu[i] = -1;
            }
            for (j = 0; j < primes_cnt; ++j) {
                uint32_t p = primes[j];
                uint64_t ip = (uint64_t)i * p;
                if (ip > limit) break;
                is_comp[ip] = 1;
                if (i % p == 0) {
                    mu[ip] = 0;
                    break;
                } else {
                    mu[ip] = (int8_t)-mu[i];
                }
            }
        }
        free(is_comp);
    }

    /* --- Compute sigma by divisor accumulation --- */
    for (i = 0; i <= limit; ++i) sigma[i] = 0;
    for (i = 1; i <= limit; ++i) {
        for (j = i; j <= limit; j += i) sigma[j] += i;
    }

    /* --- Compute prefix sums a(k) --- */
    prefix[0] = (seq_t)0;
    for (i = 1; i <= limit; ++i) {
        seq_t term = (seq_t)mu[i] * (seq_t)sigma[i];
        prefix[i] = prefix[i-1] + term;
    }
    max_n = limit;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    size_t un = (size_t)n;
    if (un > max_n) {
        expand_and_compute(un);
    }
    return prefix[un];
}
