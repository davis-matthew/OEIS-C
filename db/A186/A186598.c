/*
 * A186598  Number of (n+3) X 11 0..3 arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A186598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for a(n) for n>18: a(n) = 2*a(n-1) – a(n-2)
 *            + 3*a(n-4) – 6*a(n-5) + 3*a(n-6) + 3*a(n-8) – 3*a(n-9) Initial
 *            values (1 ≤ n ≤ 18): a(1)= 78728809 a(2)= 1184226 a(3)= 1922661
 *            a(4)= 5769381 a(5)=12653493 a(6)=17171973 a(7)=17856813
 *            a(8)=20385141 a(9)=23590989 a(10)=27166341 a(11)=...
 * Range:     u8 none, u16 none, u32 n <= 23, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const seq_t init[19] = {
    0ULL,          /* dummy for 0-index */
    78728809ULL,   /* a(1) */
    1184226ULL,    /* a(2) */
    1922661ULL,    /* a(3) */
    5769381ULL,    /* a(4) */
    12653493ULL,   /* a(5) */
    17171973ULL,   /* a(6) */
    17856813ULL,   /* a(7) */
    20385141ULL,   /* a(8) */
    23590989ULL,   /* a(9) */
    27166341ULL,   /* a(10) */
    31913637ULL,   /* a(11) */
    43392285ULL,   /* a(12) */
    72170517ULL,   /* a(13) */
    105970461ULL,  /* a(14) */
    144672021ULL,  /* a(15) */
    210476109ULL,  /* a(16) */
    337796493ULL,  /* a(17) */
    520618896ULL   /* a(18) */
};

static seq_t cache[20001];          /* Supports n up to roughly 20000 */
static idx_t cache_max = 18;       /* Highest n already computed */
static int  cache_initialized = 0; /* Flag indicating cache has been primed */

/*  Compute a(n) for n >= 1.  Cumulative values are memoised so
    calls in arbitrary order are efficient. */
seq_t a(idx_t n)
{
    if (n <= 18)
        return init[n];

    if (!cache_initialized) {
        /* Initialise cache with zeroes and then the known initial terms. */
        for (size_t i = 0; i < sizeof(cache)/sizeof(cache[0]); ++i)
            cache[i] = 0;
        for (size_t i = 1; i <= 18 && i < sizeof(cache)/sizeof(cache[0]); ++i)
            cache[i] = init[i];
        cache_initialized = 1;
    }

    if (n > cache_max) {
        /* Compute all missing terms up to n and store them. */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t val = 2*cache[i-1];
            val -= cache[i-2];
            val += 3*cache[i-4];
            val -= 6*cache[i-5];
            val += 3*cache[i-6];
            val += 3*cache[i-8];
            val -= 3*cache[i-9];
            cache[i] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
