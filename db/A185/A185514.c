/*
 * A185514  Number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock
 *          singular.
 * https://oeis.org/A185514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A185514
   Recurrence: a(n) = 12*a(n‑1) – 11*a(n‑2) – 154*a(n‑3) + 224*a(n‑4)
   Initial values: a(1)=143, a(2)=1489, a(3)=12361, a(4)=115979
   Offset: 1   (a(1) is the first term listed) */
#include <stdlib.h>
#include <stdbool.h>

/* Return the n-th term of A185514.  n>=1. */
seq_t a(idx_t n)
{
    /* Dummy value for a(0) – the sequence is 1‑based. */
    if (n == 0)
        return 0;

    /* Static cache: allocated once, grows on demand. */
    static seq_t *cache   = NULL;   /* array of a(0)..a(k) */
    static size_t cache_cap = 0;    /* number of elements allocated */

    /* First time: initialize cache with base values up to a(4). */
    if (cache == NULL) {
        cache_cap = 5; /* indices 0..4 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure */

        cache[0] = (seq_t)0;      /* unused placeholder */
        cache[1] = (seq_t)143;
        cache[2] = (seq_t)1489;
        cache[3] = (seq_t)12361;
        cache[4] = (seq_t)115979;
    }

    /* Grow the cache if the requested index is outside the current range. */
    if ((size_t)n + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        cache_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        cache = tmp;

        /* Compute any new terms that appear after the reallocation. */
        for (size_t i = old_cap; i <= (size_t)n; ++i) {
            seq_t t1 = cache[i - 1] * (seq_t)12;
            seq_t t2 = cache[i - 2] * (seq_t)11;
            seq_t t3 = cache[i - 3] * (seq_t)154;
            seq_t t4 = cache[i - 4] * (seq_t)224;
            cache[i] = t1 - t2 - t3 + t4;
        }
    } else {
        /* Cache already large enough; compute missing terms if any. */
        size_t start = cache_cap;           /* first uncomputed index */
        if (start <= (size_t)n) {
            for (size_t i = start; i <= (size_t)n; ++i) {
                seq_t t1 = cache[i - 1] * (seq_t)12;
                seq_t t2 = cache[i - 2] * (seq_t)11;
                seq_t t3 = cache[i - 3] * (seq_t)154;
                seq_t t4 = cache[i - 4] * (seq_t)224;
                cache[i] = t1 - t2 - t3 + t4;
            }
            cache_cap = n + 1; /* now we have terms up to n */
        }
    }

    return cache[(size_t)n];
}
