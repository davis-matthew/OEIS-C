/*
 * A021268  Expansion of 1/((1-x)(1-2x)(1-8x)(1-10x)).
 * https://oeis.org/A021268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 136*a(n-2) + 276*a(n-3) - 160*a(n-4) for n>3
 *            with initial values a(0)=1, a(1)=21, a(2)=305, a(3)=3825
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

seq_t a(idx_t n) {
    /* First call: allocate cache and set initial terms */
    if (cache_len == 0) {
        size_t initcap = (size_t)((n >= 3) ? n + 1 : 4);
        seq_cache = (seq_t *)malloc(initcap * sizeof(seq_t));
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)21;
        seq_cache[2] = (seq_t)305;
        seq_cache[3] = (seq_t)3825;
        cache_len = 4;
        cache_cap = initcap;
    }

    /* Grow cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
        seq_cache = tmp;
        cache_cap = newcap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            seq_cache[0] = (seq_t)1;
        } else if (i == 1) {
            seq_cache[1] = (seq_t)21;
        } else if (i == 2) {
            seq_cache[2] = (seq_t)305;
        } else if (i == 3) {
            seq_cache[3] = (seq_t)3825;
        } else {
            seq_cache[i] = 21 * seq_cache[i - 1]
                         - 136 * seq_cache[i - 2]
                         + 276 * seq_cache[i - 3]
                         - 160 * seq_cache[i - 4];
        }
    }
    if ((size_t)n + 1 > cache_len) {
        cache_len = (size_t)n + 1;
    }
    return seq_cache[n];
}
