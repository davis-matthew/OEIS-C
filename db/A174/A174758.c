/*
 * A174758  x-values in the solution to x^2-55*y^2=1.
 * https://oeis.org/A174758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 178*a(n-1) - a(n-2), a(1)=1, a(2)=89
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 0 (undefined) */
    if (n == 0) return 0;

    /* Static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;
    static idx_t  cap  = 0;          /* number of elements stored in cache */

    /* First call: allocate and fill the cache up to max(n,2) */
    if (cap == 0) {
        idx_t needed = (n < 2) ? 2 : n;          /* at least two terms */
        cache = (seq_t*)malloc(needed * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;    /* a(1) */
        cache[1] = (seq_t)89;   /* a(2) */
        for (idx_t i = 3; i <= needed; ++i) {
            cache[i-1] = ((seq_t)178 * cache[i-2]) - cache[i-3];
        }
        cap = needed;
        return cache[n-1];
    }

    /* Return cached value if already computed */
    if (n <= cap) return cache[n-1];

    /* Grow the cache to accommodate the new size n */
    idx_t needed = n;
    seq_t *new_cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cap + 1; i <= n; ++i) {
        cache[i-1] = ((seq_t)178 * cache[i-2]) - cache[i-3];
    }
    cap = needed;
    return cache[n-1];
}
