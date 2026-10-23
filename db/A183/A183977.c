/*
 * A183977  1/4 the number of (n+1) X (n+1) binary arrays with all 2 X 2
 *          subblock sums the same.
 * https://oeis.org/A183977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-3) + 4*a(n-4) (valid for n >= 5).
 *            Initial values: a(1) = 4, a(2) = 8, a(3) = 14, a(4) = 26
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy, growing cache: cache[i] = a(i), 1‑based */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated capacity (indices 0..cap-1) */
    static size_t len = 0;      /* number of values stored (>=4 after init) */

    /* Basic sanity: ignore requests below offset */
    if (n < 1)
        return (seq_t)0;

    /* Initialise cache on first call */
    if (len == 0) {
        cap = 8;                                   /* start with room for n up to 7 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                       /* out of memory – return 0 */

        /* cache[0] unused; values from 1 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)26;
        len = 4;                                   /* we have a(1..4) */
    }

    /* If requested value already computed, return it */
    if (n <= len)
        return cache[n];

    /* Grow the cache to fit the requested index (ensure capacity for n) */
    size_t need = (size_t)n;
    if (need + 1 > cap) {                       /* +1 because we use 1‑based */
        size_t newcap = cap;
        while (newcap < need + 1)
            newcap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)                                 /* realloc failed */
            return (seq_t)0;

        cache = tmp;
        cap   = newcap;
    }

    /* Compute values from len+1 up to n via the recurrence */
    for (size_t i = len + 1; i <= need; ++i) {
        if (i <= 4) {         /* should not happen, but keep for safety */
            /* values already known */
        } else {
            seq_t t1 = cache[i-1] * (seq_t)3;        /* 3*a(i-1) */
            seq_t t2 = cache[i-3] * (seq_t)6;        /* 6*a(i-3) */
            seq_t t3 = cache[i-4] * (seq_t)4;        /* 4*a(i-4) */
            cache[i] = t1 - t2 + t3;                 /* a(i) = 3a_{i-1} - 6a_{i-3} + 4a_{i-4} */
        }
    }

    len = need;                                   /* update stored length */
    return cache[(size_t)n];
}
