/*
 * A183996  Number of (n+1) X 3 0..2 arrays with all 2 X 2 subblock sums the
 *          same.
 * https://oeis.org/A183996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *  a(1)=171, a(2)=309, a(3)=579, a(4)=1233, a(5)=2691,
 *  a(6)=6417, a(7)=15555, a(8)=40041, a(9)=104211
 *  a(n) = 6*a(n‑1) – 60*a(n‑3) + 85*a(n‑4) + 150*a(n‑5)
 *         – 360*a(n‑6) + 396*a(n‑8) – 216*a(n‑9)   (n ≥ 10)
 */

#include <stdlib.h>

/* Do not (re)define idx_t or seq_t – they are supplied by the build. */

/* Initial values for n = 1 … 9 */
static const seq_t init_vals[9] = {
    (seq_t)171,      /* a(1) */
    (seq_t)309,      /* a(2) */
    (seq_t)579,      /* a(3) */
    (seq_t)1233,     /* a(4) */
    (seq_t)2691,     /* a(5) */
    (seq_t)6417,     /* a(6) */
    (seq_t)15555,    /* a(7) */
    (seq_t)40041,    /* a(8) */
    (seq_t)104211    /* a(9) */
};

seq_t a(idx_t n)
{
    /* Valid OEIS indices start at 1. */
    if (n < (idx_t)1) {
        return (seq_t)0; /* undefined for n < 1 */
    }

    /* For the first nine terms, return the initial table. */
    if (n <= (idx_t)9) {
        return init_vals[n - (idx_t)1];
    }

    /* Static cache that grows as needed. */
    static seq_t *cache = NULL;    /* stores a(1)…a(current_len) */
    static idx_t cache_len = (idx_t)0;   /* number of stored terms */

    /* Ensure the cache has at least nine entries. */
    if (cache_len < (idx_t)9) {
        cache_len = (idx_t)9;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();   /* allocation failure */
        for (idx_t i = (idx_t)0; i < cache_len; ++i) {
            cache[i] = init_vals[i];
        }
    }

    /* Extend the cache up to the requested index n. */
    if (n > cache_len) {
        /* Reallocate to accommodate n terms. */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = cache_len; i < n; ++i) {
            /* compute a(i+1) using the recurrence; i is zero‑based index */
            seq_t pos = 6 * cache[i - (idx_t)1]
                      + 85 * cache[i - (idx_t)4]
                      + 150 * cache[i - (idx_t)5]
                      + 396 * cache[i - (idx_t)8];

            seq_t neg = 60 * cache[i - (idx_t)3]
                      + 360 * cache[i - (idx_t)6]
                      + 216 * cache[i - (idx_t)9];

            cache[i] = pos - neg;
        }
        cache_len = n;
    }

    return cache[n - (idx_t)1];
}
