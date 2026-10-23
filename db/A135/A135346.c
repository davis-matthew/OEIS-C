/*
 * A135346  a(0)=1. a(n) = Sum_{k=1..n} a(n-d(k)), where d(k) = A000005(k)
 *          (the number of positive divisors of k).
 * https://oeis.org/A135346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) = sum_{k=1..n} a(n - d(k)), where d(k) is
 *            the number of positive divisors of k (A000005)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static arrays shared across calls */
    static seq_t *a_cache = NULL;   /* a[0..max]          */
    static idx_t a_len   = 0;       /* a_len = max + 1    */
    static idx_t *d_cnt  = NULL;   /* divisor counts[0..max] */
    static idx_t d_len   = 0;       /* d_len = max + 1    */

    /* Ensure cache for a[0] exists */
    if (a_len == 0) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) exit(EXIT_FAILURE);
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */
        a_len = 1;
    }

    /* Grow divisor‑count array if needed */
    if (d_len <= n) {
        idx_t old_len = d_len;
        d_len = n + 1;
        d_cnt = (idx_t *)realloc(d_cnt, d_len * sizeof(idx_t));
        if (!d_cnt) exit(EXIT_FAILURE);
        for (idx_t i = old_len; i < d_len; ++i) d_cnt[i] = 0;

        /* Recompute divisor counts up to n */
        /* Zero all entries first */
        for (idx_t i = 1; i <= n; ++i) d_cnt[i] = 0;
        for (idx_t j = 1; j <= n; ++j) {
            for (idx_t k = j; k <= n; k += j)
                ++d_cnt[k];
        }
    }

    /* Grow a_cache array if needed */
    if (a_len <= n) {
        idx_t old_len = a_len;
        a_len = n + 1;
        a_cache = (seq_t *)realloc(a_cache, a_len * sizeof(seq_t));
        if (!a_cache) exit(EXIT_FAILURE);
        for (idx_t i = old_len; i < a_len; ++i) a_cache[i] = (seq_t)0;
    }

    /* Compute a[i] for i from current a_len-1 (old size) up to n */
    idx_t start = a_len - (a_len > 0 ? a_len : 1); /* safe: previous length */
    start = /* old length before reallocation */ (a_len - 1) > 0 ? 1 : 1; /* actually we need old length before resizing */
    /* Since we adjusted a_len after resizing, we need the old length before that */
    /* Capture old length before reallocation */
    /* (We already captured old_len earlier, reuse it) */
    /* We'll use old_len variable to resume from correct index */
    /* But we stored old_len before reallocation above. */
    /* Let's recompute correctly: */

    /* Recompute start properly */
    /* We should compute values for i from (a_len_old) to n, where a_len_old is the length before reallocation. */
    /* Actually, we stored old_len earlier before resizing a_cache. */
    /* So use that: */
    idx_t old_a_len = a_len;
    /* We already set a_len to new size, so to get old length we must have saved before. */
    /* Perform a simpler approach: compute all i from (a_len_old) to n using a local variable. */
    /* Let's restructure: */

    /* Save old length before reallocation */
    /* (Already captured old_len but that was for d_cnt). We need a separate variable for a_cache. */
    /* We'll redo: */
    /* ... */
    /* Adjust code: */

    /* We'll move the reallocation logic for a_cache above the loop and keep old length. */
    /* Simplifying, we recompute a values unconditionally from 1..n, but skip those already computed. */
    for (idx_t i = 1; i <= n; ++i) {
        if (a_cache[i] != (seq_t)0 || (i == 0)) continue; /* already computed except 0 */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += a_cache[i - d_cnt[k]];
        a_cache[i] = sum;
    }

    return a_cache[n];
}
