/*
 * A085776  Numbers k such that k concatenated with k+1 is a semiprime.
 * https://oeis.org/A085776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085776: Numbers k such that the decimal concatenation of k and k+1
   is a semiprime (product of exactly two primes, allowing a square).  The
   function computes terms by sequential search, storing already found terms
   in a static cache so that calls with arbitrary indices are fast. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Factorisation routine for 64‑bit integers. */
#include "../utils/factor_uint64.h"

/* ------------------------------------------------------------ */
/* Helpers                                                          */
/* ------------------------------------------------------------ */

/* Number of decimal digits of a 64‑bit unsigned integer */
static int digits_uint64(uint64_t x)
{
    if (x == 0) return 1;
    int d = 0;
    while (x > 0) {
        x /= 10;
        d++;
    }
    return d;
}

/* Test whether a 64‑bit integer is a semiprime (product of exactly two
   primes, allowing squares). */
static bool is_semiprime_uint64(uint64_t n)
{
    uint64_t fac[20];
    int      exp[20];
    int      fcnt = 0;

    factor_uint64(n, fac, exp, &fcnt);
    if (fcnt == 1) return exp[0] == 2;          /* p^2                */
    if (fcnt == 2) return exp[0] == 1 && exp[1] == 1;  /* p*q               */
    return false;                               /* otherwise          */
}

/* ------------------------------------------------------------ */
/* Cache for computed terms                                       */
/* ------------------------------------------------------------ */

static uint64_t *cache      = NULL;   /* array holding known a(n) values */
static size_t   cache_len   = 0;      /* number of terms stored          */
static const size_t CACHE_CAP = 20000;/* generous capacity for safety     */
static uint64_t next_candidate = 1;   /* next k to test, continues across calls */

/* Initialise the cache on first use. */
static void init_cache(void)
{
    if (cache == NULL) {
        cache = (uint64_t*)malloc(CACHE_CAP * sizeof(uint64_t));
    }
}

/* Append a new term to the cache.  The cache has enough capacity so
   that overflow does not occur for the required range (n <= 10000). */
static void append_term(uint64_t val)
{
    if (cache_len < CACHE_CAP) {
        cache[cache_len++] = val;
    }
}

/* ------------------------------------------------------------ */
/* Main function                                                   */
/* ------------------------------------------------------------ */

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range, but defined behaviour */

    init_cache();

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        uint64_t k   = next_candidate;
        uint64_t kp1 = k + 1;

        /* Compute 10^d where d is number of digits of k+1 */
        int dlen = digits_uint64(kp1);
        unsigned __int128 pow10 = 1;
        for (int i = 0; i < dlen; ++i) pow10 *= 10;

        /* Concatenation: k * 10^d + (k+1)   (use 128‑bit to detect overflow) */
        unsigned __int128 concat128 = ((__int128)k) * pow10 + kp1;

        /* Only test numbers that fit in 64 bits.  Larger numbers are
           ignored; for the range up to the 10000th term this will
           never be needed. */
        if (concat128 <= UINT64_MAX)
        {
            uint64_t concat64 = (uint64_t)concat128;
            if (is_semiprime_uint64(concat64))
                append_term(k);
        }

        ++next_candidate;
    }

    /* Convert stored 64‑bit value to seq_t before returning. */
    return (seq_t)cache[n - 1];
}
