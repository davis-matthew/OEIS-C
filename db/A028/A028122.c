/*
 * A028122  Expansion of 1/((1-4x)(1-5x)(1-8x)(1-11x)).
 * https://oeis.org/A028122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=28, a(2)=505, a(3)=7500
 *            a(n)=28*a(n-1)-279*a(n-2)+1172*a(n-3)-1760*a(n-4) for n>=4
 *            (recurrence from the OEIS entry)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* array of computed terms */
static idx_t  cache_len = 0;    /* number of valid entries in cache */

/* Initial values for the first four terms */
static const seq_t init_vals[4] = {1, 28, 505, 7500};

seq_t a(idx_t n)
{
    /* handle the base cases directly */
    if (n < 4)
        return init_vals[n];

    /* ensure the cache has space and base terms stored */
    if (cache_len < 4) {
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        /* store the first four terms */
        cache[0] = init_vals[0];
        cache[1] = init_vals[1];
        cache[2] = init_vals[2];
        cache[3] = init_vals[3];
    }

    /* need to compute further terms up to n? */
    if (cache_len <= n) {
        idx_t needed = n + 1;                 /* total entries required */
        if (needed > cache_len) {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
            /* compute new terms */
            for (idx_t i = cache_len; i < needed; ++i) {
                seq_t term = 28 * cache[i - 1]
                          - 279 * cache[i - 2]
                          + 1172 * cache[i - 3]
                          - 1760 * cache[i - 4];
                cache[i] = term;
            }
            cache_len = needed;
        }
    }

    return cache[n];
}
