/*
 * A141609  a(n) = (a(n-1)*a(n-2) + a(n-1)^2)/a(n-3), with a(1) = a(2) = a(3)
 *          = 1.
 * https://oeis.org/A141609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = a(2) = a(3) = 1,
 * a(n) = (a(n-1) * a(n-2) + a(n-1) * a(n-1)) / a(n-3)   for n > 3 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) /* sequence undefined for n = 0, return 0 to be safe */
        return (seq_t)0;

    /* Static cache for memoization (1‑based indexing). */
    static seq_t *cache      = NULL; /* array of seq_t */
    static idx_t cache_len   = 0;    /* highest index stored */
    static idx_t cache_cap   = 0;    /* allocated capacity */

    /* Make sure we have a small initial allocation. */
    if (cache_cap < 4) {
        idx_t new_cap = 4;          /* allocate for indices 0..3 */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure handled gracefully */
        cache = tmp;
        cache_cap = new_cap;
        if (cache_len == 0) {
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)1;
            cache[3] = (seq_t)1;
            cache_len = 3;
        }
    }

    /* Return cached value if already computed. */
    if (n <= cache_len)
        return cache[n];

    /* Grow the cache if necessary to include index n. */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;      /* to use 1‑based indexing */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms sequentially up to n, storing them. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];   /* a(i‑1) */
        seq_t a2 = cache[i - 2];   /* a(i‑2) */
        seq_t a3 = cache[i - 3];   /* a(i‑3) */
        cache[i] = (a1 * a2 + a1 * a1) / a3;
    }
    cache_len = n;
    return cache[n];
}
