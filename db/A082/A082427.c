/*
 * A082427  a(1)=1, a(n) = n * (Sum_{k=1..n-1} a(k)) - 2.
 * https://oeis.org/A082427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (Sum_{k=1}^{n-1} a(k)) - 2 * Starting value: a(1) = 1
 *            * The recurrence can be computed incrementally while keeping a
 *            running * partial sum; this avoids intermediate products that
 *            exceed seq_t * even when the final result fits in seq_t (e.g.
 *            for the 64‑bit build)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Memoisation cache for already computed terms */
    static seq_t *cache = NULL;    /* 1‑based index: cache[1] = a(1) */
    static idx_t cache_len = 0;    /* highest index already computed */
    static seq_t cumulative_sum = 0;  /* Sum_{k=1}^{i} a(k) for i = cache_len */

    if (n == 0)                /* the sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    /* Initialise cache for the first call */
    if (cache_len == 0) {
        /* Allocate space for indices 1 and 2 (we will grow as needed) */
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[1] = (seq_t)1;               /* a(1) */
        cumulative_sum = cache[1];
        cache_len = 1;
    }

    /* Grow cache and compute missing values up to n */
    if (n > cache_len) {
        idx_t new_len = n + 1;  /* +1 because we use 1‑based index */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;

        /* Start computing from cache_len+1 up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t term = (seq_t)i * cumulative_sum - (seq_t)2;
            cache[i] = term;
            cumulative_sum += term;
        }
        cache_len = n;
    }

    return cache[n];
}
