/*
 * A204710  Number of (n+1) X 6 0..1 arrays with the permanents of all 2 X 2
 *          subblocks equal and nonzero.
 * https://oeis.org/A204710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 15*a(n-2) - 38*a(n-3) - 52*a(n-4) + 72*a(n-5),
 *            with a(1)=97, a(2)=529, a(3)=2689, a(4)=14145, a(5)=73345. The
 *            recurrence is assumed to hold for all n≥6. This implementation
 *            memoises computed values so that repeated calls in any order
 *            share the same cache. All arithmetic is perfo...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms (1‑based indexing) */
    const seq_t INIT[5] = {97, 529, 2689, 14145, 73345};

    /* Static cache: cache[i‑1] = a(i)  for 1 ≤ i ≤ cache_len */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Request for a(n) with n < 1 is undefined; assume caller never does that. */
    if (n <= 5)
        return INIT[n - 1];

    /* Allocate / grow the cache on first use or when needed */
    if (cache == NULL) {
        cache_cap = (n > 5 ? n : 5) + 5;      /* extra room for growth */
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = INIT[i];
        cache_len = 5;
    } else if (n > cache_cap) {
        /* Grow the capacity (doubling strategy) */
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms up to n if not already computed */
    if (n > cache_len) {
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            /* k‑th term depends on previous five terms:
               a(k) = 4*a(k‑1) + 15*a(k‑2) - 38*a(k‑3) - 52*a(k‑4) + 72*a(k‑5) */
            seq_t a1 = cache[k - 2]; /* a(k‑1) */
            seq_t a2 = cache[k - 3]; /* a(k‑2) */
            seq_t a3 = cache[k - 4]; /* a(k‑3) */
            seq_t a4 = cache[k - 5]; /* a(k‑4) */
            seq_t a5 = cache[k - 6]; /* a(k‑5) */
            seq_t val = 4 * a1 + 15 * a2 - 38 * a3 - 52 * a4 + 72 * a5;
            cache[k - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
