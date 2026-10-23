/*
 * A186592  Number of (n+3)X5 0..3 arrays with every 4X4 subblock commuting
 *          with each horizontal and vertical neighbor 4X4 subblock.
 * https://oeis.org/A186592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 30, u64 n <= 86, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A186592
    Empirical recurrence for n > 14:
        a(n) = 2*a(n-1) - a(n-2) + 3*a(n-4) - 6*a(n-5) + 3*a(n-6)
               + 3*a(n-8) - 3*a(n-9)
    Initial values (a(1) … a(14)):
        48243, 2703, 8853, 104547, 444066, 884466, 1126074,
        1184226, 1210122, 1236594, 1282434, 1586349, 2884656, 5616483
*/

#include <stddef.h>
#include <stdlib.h>

const seq_t init_vals[14] = {
    48243ULL, 2703ULL, 8853ULL, 104547ULL,
    444066ULL, 884466ULL, 1126074ULL, 1184226ULL,
    1210122ULL, 1236594ULL, 1282434ULL, 1586349ULL,
    2884656ULL, 5616483ULL
};

static seq_t *cache = NULL;
static size_t cache_len = 0;  /* number of values stored */
static size_t cache_cap = 0;  /* capacity of cache array */

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* undefined for n<1 */

    /* If n is within the initial values, return directly */
    if (n <= 14) {
        return init_vals[(size_t)n - 1];
    }

    /* Prepare cache if it doesn't exist yet */
    if (cache == NULL) {
        /* Allocate at least 14 elements, or n if larger */
        cache_cap = (n > 14) ? (size_t)n : 14;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed */
        for (size_t i = 0; i < 14; ++i) cache[i] = init_vals[i];
        cache_len = 14;
    }

    /* Expand cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* reallocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        /* Indices are zero-based: cache[k] holds a(k+1) */
        seq_t val = (seq_t)2 * cache[i - 2];   /* 2*a(n-1) */
        val -= cache[i - 3];                  /* - a(n-2) */
        val += (seq_t)3 * cache[i - 5];       /* + 3*a(n-4) */
        val -= (seq_t)6 * cache[i - 6];       /* - 6*a(n-5) */
        val += (seq_t)3 * cache[i - 7];       /* + 3*a(n-6) */
        val += (seq_t)3 * cache[i - 9];       /* + 3*a(n-8) */
        val -= (seq_t)3 * cache[i - 10];      /* - 3*a(n-9) */
        cache[i - 1] = val;                   /* store a(i) */
    }
    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
