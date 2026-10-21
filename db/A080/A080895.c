/*
 * A080895  Expansion of the exponential series exp( x R(x) ) = exp((1 + x -
 *          sqrt(1 - 2 x - 3x^2))/(2(1 + x))), where R(x) is the ordinary
 *          generating series of the Riordan numbers A005043.
 * https://oeis.org/A080895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) satisfies a(0)=1, a(1)=1, a(2)=1, a(3)=7, and for n≥0
 *            a(n+4) = 3*n*(n+1)^2*(n+2)*a(n) + (n+2)*(12+20n+8n^2)*a(n+1) +
 *            (25+24n+6n^2)*a(n+2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
/* Helper allocation routines from the test harness */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_cache.h"

static seq_t *cache = NULL;      /* cached terms                      */
static size_t cache_cap = 0;      /* allocated capacity in terms        */
static size_t cache_len = 0;      /* number of terms already filled     */

/* Ensure that cache contains entries up to at least index n. */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_len) return;            /* already enough */

    /* Expand storage if needed.  Grow by factor 2 or enough for n+1. */
    if (!cache) {
        cache_cap = n + 1;
        if (cache_cap < 16) cache_cap = 16;
    } else if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache_cap = new_cap;
    }

    /* Allocate new buffer and copy old terms. */
    seq_t *new_cache = (seq_t *)alloc_seq_array(cache_cap);
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    if (cache) free_cache(cache);
    cache = new_cache;

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)            cache[0] = (seq_t)1;
        else if (i == 1)       cache[1] = (seq_t)1;
        else if (i == 2)       cache[2] = (seq_t)1;
        else if (i == 3)       cache[3] = (seq_t)7;
        else {
            idx_t m = i - 4;   /* n in the recurrence */
            seq_t a0 = cache[m];
            seq_t a1 = cache[m+1];
            seq_t a2 = cache[m+2];

            seq_t coeff1 = (seq_t)3 * (seq_t)m * (seq_t)(m+1) * (seq_t)(m+1) * (seq_t)(m+2);
            seq_t coeff2 = (seq_t)(m+2) *
                           ( (seq_t)12 + (seq_t)20 * m + (seq_t)8 * m * m );
            seq_t coeff3 = (seq_t)25 + (seq_t)24 * m + (seq_t)6 * m * m;

            cache[i] = a0 * coeff1 + a1 * coeff2 + a2 * coeff3;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n);
    return cache[n];
}
