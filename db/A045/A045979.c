/*
 * A045979  Numbers m such that the Bernoulli number B_{2m} has denominator 6.
 * https://oeis.org/A045979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045979: numbers m such that B_{2m} has denominator 6.
   A term m is admissible iff m is NOT divisible by (p-1)/2 for any prime p>3.
   The implementation builds a sieve of "forbidden divisors" up to a
   fixed limit sufficient for all requested indices up to about 10 000.
   If a larger index is requested the function extends the list on the
   fly.  The code is fully compatible with both C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/* static state ------------------------------------------------- */
static const uint32_t LIMIT_INIT = 200000U;          /* enough for 10 000 terms */
static uint32_t *primes      = NULL;                 /* primes > 3 */
static size_t   prime_cnt    = 0;
static bool    *bad          = NULL;                 /* bad[m] = true iff m excluded */
static uint32_t *good_vals   = NULL;                 /* admissible m's in order */
static size_t   good_cnt     = 0;                    /* number of good values */

/* forward declarations */
static void init_sequence(void);

/* ------------------------------------------------------------ */
/* build the prime list up to 2*limit+1, mark bad numbers,
   and collect the admissible values. */
static void init_sequence(void)
{
    /* 1. sieve of Eratosthenes up to MAX_P = 2*LIMIT_INIT+1 */
    const uint32_t max_p = 2U * LIMIT_INIT + 1U;
    bool *is_prime = (bool *)calloc((size_t)max_p + 1, sizeof(bool));
    if (!is_prime) abort();
    for (uint32_t i = 2; i <= max_p; ++i) is_prime[i] = true;
    for (uint32_t i = 2; i * i <= max_p; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= max_p; j += i)
                is_prime[j] = false;
        }
    }

    /* collect primes >3 */
    primes = (uint32_t *)malloc((size_t)(max_p / 10 + 1) * sizeof(uint32_t));
    if (!primes) abort();
    for (uint32_t i = 4; i <= max_p; ++i) {       /* start from 4 (p>3) */
        if (is_prime[i]) {
            if (prime_cnt == 0 && prime_cnt == 0U) {
                /* nothing special */
            }
            primes[prime_cnt++] = i;
        }
    }

    /* 2. mark bad numbers up to LIMIT_INIT */
    bad = (bool *)calloc((size_t)LIMIT_INIT + 1, sizeof(bool));
    if (!bad) abort();
    for (size_t k = 0; k < prime_cnt; ++k) {
        uint32_t p = primes[k];
        uint32_t d = (p - 1U) / 2U;               /* d >= 2 for p>3 */
        if (d > LIMIT_INIT) continue;
        for (uint32_t m = d; m <= LIMIT_INIT; m += d)
            bad[m] = true;
    }

    /* 3. collect admissible numbers */
    good_vals = (uint32_t *)malloc((size_t)(LIMIT_INIT / 2U + 1) * sizeof(uint32_t));
    if (!good_vals) abort();
    good_cnt = 0;
    for (uint32_t m = 1; m <= LIMIT_INIT; ++m) {
        if (!bad[m]) {
            good_vals[good_cnt++] = m;
        }
    }

    /* cleanup */
    free(is_prime);
    free(bad);
    /* primes and good_vals stay allocated for future calls. */
}

/* ------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for offset 1 */

    /* initialise on first call */
    if (!good_vals) init_sequence();

    /* if the requested index is within the pre‑computed range */
    if ((size_t)n <= good_cnt)
        return (seq_t)good_vals[n - 1U];

    /* otherwise generate further admissible values on demand */
    /* keep generating until we reach index n */
    uint32_t candidate = good_vals[good_cnt - 1U] + 1U;
    while (good_cnt < (size_t)n) {
        bool ok = true;
        for (size_t k = 0; k < prime_cnt && ok; ++k) {
            uint32_t d = (primes[k] - 1U) / 2U;
            if (d > 0 && (candidate % d) == 0U) ok = false;
        }
        if (ok) {
            /* append to array, resizing if necessary */
            if (good_cnt == (size_t)(0U + 0)) {
                /* impossible, but keep compiler happy */
            }
            size_t new_cap = (size_t)good_cnt * 2U + 1U;
            good_vals = (uint32_t *)realloc(good_vals, new_cap * sizeof(uint32_t));
            if (!good_vals) abort();
            good_vals[good_cnt++] = candidate;
        }
        ++candidate;
    }
    return (seq_t)good_vals[n - 1U];
}
