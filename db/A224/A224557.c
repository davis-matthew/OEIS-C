/*
 * A224557  Number of (n+2) X 8 0..1 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6) for n > 8; a(1)=418, a(2)=351, a(3)=451, a(4)=551,
 *            a(5)=740, a(6)=992, a(7)=1298, a(8)=1742
 * Range:     u8 none, u16 n <= 18, u32 n <= 47, u64 n <= 105, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Initial values for 1 <= n <= 8 */
    const seq_t init[8] = {
        (seq_t)418, (seq_t)351, (seq_t)451, (seq_t)551,
        (seq_t)740, (seq_t)992, (seq_t)1298, (seq_t)1742
    };

    if (n == (idx_t)0) return (seq_t)0;          /* undefined, safety */
    if (n <= (idx_t)8) return init[n-1];

    /* Memoization cache: cache[i] stores a(i+1) */
    static seq_t *cache    = NULL;
    static size_t  cap     = 0;    /* allocated size */
    static size_t  cache_sz= 0;    /* highest n for which cache is filled */

    /* First-time initialization */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure, safety */
        for (idx_t i = 0; i < (idx_t)8; ++i) cache[i] = init[i];
        cache_sz = 8;
    }

    /* Grow cache if needed */
    if ((size_t)n > cache_sz) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) new_cap = new_cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure, safety */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_sz; i < (size_t)n; ++i) {
        seq_t a1 = cache[i-1];          /* a(i+1) corresponds to cache[i] */
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t a5 = cache[i-5];
        seq_t a6 = cache[i-6];

        /* Positive parts of the recurrence: avoid intermediate negative values */
        seq_t sum1 = 4 * a1;
        sum1 += 5 * a3;
        sum1 += 3 * a5;

        seq_t sum2 = 6 * a2;
        sum2 += 4 * a4;
        sum2 += a6;

        cache[i] = sum1 - sum2;
    }

    cache_sz = n;
    return cache[n-1];
}
