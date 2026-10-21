/*
 * A216861  a(n) = 13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 182*a(n-4) + 91*a(n-5)
 *          - 13*a(n-6), with initial terms 0, -2, -9, -44, -215, -1001.
 * https://oeis.org/A216861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 182*a(n-4) +
 *            91*a(n-5) - 13*a(n-6) * Initial values: a(1)=0, a(2)=-2,
 *            a(3)=-9, a(4)=-44, a(5)=-215, a(6)=-1001
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 32, i128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;
static size_t cache_cap = 0;   /* allocated size */
static int    initialized = 0; /* 0 = not yet initialized */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* the sequence is defined for n ≥ 1; 0 returned for safety */
    }

    size_t need = (size_t)n;   /* convert index to size_t for array math */

    /* First time: allocate and initialise the buffer */
    if (!initialized) {
        cache_cap = (need < 6) ? 6 : need;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            return 0;   /* allocation failure – return a safe value */
        }
        /* initialise the first six terms */
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)-2;  /* a(2) */
        cache[2] = (seq_t)-9;  /* a(3) */
        cache[3] = (seq_t)-44; /* a(4) */
        cache[4] = (seq_t)-215;/* a(5) */
        cache[5] = (seq_t)-1001;/* a(6) */
        initialized = 1;
    }
    /* Grow the buffer if necessary */
    else if (cache_cap < need) {
        size_t new_cap = cache_cap;
        while (new_cap < need) {
            new_cap <<= 1;   /* double until big enough */
        }
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            return 0;   /* allocation failure */
        }
        for (size_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = cache[i];            /* copy existing terms */
        }
        free(cache);
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute any missing terms up to 'need' */
    for (size_t i = 6; i < need; ++i) {
        cache[i] = 13 * cache[i - 1]
                 - 65 * cache[i - 2]
                 + 156 * cache[i - 3]
                 - 182 * cache[i - 4]
                 + 91  * cache[i - 5]
                 - 13  * cache[i - 6];
    }

    return cache[need - 1];
}
