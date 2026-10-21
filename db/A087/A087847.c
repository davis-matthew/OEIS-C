/*
 * A087847  a(n) = a(|n - a(n-1)|) + a(a(a(|n - a(n-4)|))).
 * https://oeis.org/A087847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(|n - a(n‑1)|) + a(a(a(|n - a(n‑4)|))); initial values:
 *            a(1) = a(2) = a(3) = a(4) = 1 (recurrence defined for all n ≥ 5)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   abs_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>              /* realloc, size_t */
#include "../utils/abs_diff.h"   /* idx_t abs_diff(idx_t n, seq_t v); */

seq_t a(idx_t n)
{
    /* Static cache storing a(i) for i ≥ 1 (index 0 unused) */
    static seq_t *cache   = NULL;
    static idx_t  computed = 0;   /* highest index already computed */
    static size_t cache_size = 0; /* allocated capacity */

    /* If requested value already computed, return it. */
    if (n <= computed)
        return cache[(size_t)n];

    /* Ensure the array can hold indices up to n. */
    size_t needed = (size_t)n + 1;            /* index 0 is unused */
    if (cache_size < needed)
    {
        size_t new_size = cache_size ? cache_size * 2 : needed;
        if (new_size < needed)
            new_size = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute values from computed+1 up to n. */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        seq_t val;
        if (i <= 4)            /* initial values */
        {
            val = (seq_t)1;
        }
        else
        {
            /* ---------- left part ---------- */
            idx_t left_idx  = abs_diff(i, cache[(size_t)i - 1]); /* |i - a(i-1)| */
            seq_t left_val  = cache[(size_t)left_idx];

            /* ---------- right part ---------- */
            idx_t mid_idx0 = abs_diff(i, cache[(size_t)i - 4]); /* |i - a(i-4)| */
            seq_t b = cache[(size_t)mid_idx0];
            seq_t c = cache[(size_t)b];
            seq_t d = cache[(size_t)c];

            val = left_val + d;
        }
        cache[(size_t)i] = val;
    }

    computed = n;
    return cache[(size_t)n];
}
