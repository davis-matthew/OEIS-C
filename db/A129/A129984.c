/*
 * A129984  First differences of A129983.
 * https://oeis.org/A129984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 12*a(n-3) - 3*a(n-4) - 2*a(n-5)
 *            with a(0)=0, a(1)=1, a(2)=3, a(3)=7, a(4)=15, a(5)=31
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[6] = {0, 1, 3, 7, 15, 31};

static seq_t *cache = NULL;      /* array of computed values */
static idx_t cache_size = 0;     /* number of elements currenty cached */

seq_t a(idx_t n)
{
    /* Return constant values directly for the first six indices */
    if (n < 6)
        return init_vals[(idx_t)n];

    /* Create initial cache on first use */
    if (cache == NULL) {
        cache = (seq_t*)calloc(6, sizeof(seq_t));
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init_vals[(idx_t)i];
        cache_size = 6;
    }

    /* Extend cache if we need a larger index */
    if (n >= cache_size) {
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        /* Compute missing terms using the linear recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] =
                (seq_t)6   * cache[i - 1]          /* 6*a(n-1) */
              - (seq_t)13  * cache[i - 2]          /* 13*a(n-2) */
              + (seq_t)12  * cache[i - 3]          /* 12*a(n-3) */
              - (seq_t)3   * cache[i - 4]          /* 3*a(n-4) */
              - (seq_t)2   * cache[i - 5];         /* 2*a(n-5) */
        }
        cache_size = n + 1;
    }

    return cache[(idx_t)n];
}
