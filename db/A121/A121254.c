/*
 * A121254  Number of conjugated cycles composed of six carbons in
 *          (n,n)-nanotubes in terms of the number of naphthalene units.
 * https://oeis.org/A121254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) - a(n-4) Initial values:
 *            a(1)=4, a(2)=16, a(3)=60, a(4)=208 (The sequence also equals
 *            4*A117202(n).)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms (1‑indexed). */
    static seq_t *cache     = NULL;
    static size_t  capacity = 0;   /* number of slots allocated in cache */
    static idx_t  last      = 0;   /* largest index already computed */

    /* Return directly for the first four terms. */
    if (n == 1) return 4;
    if (n == 2) return 16;
    if (n == 3) return 60;
    if (n == 4) return 208;

    /* Ensure the cache is large enough to hold indices up to n. */
    if (capacity <= (size_t)n) {
        /* Expand to at least n+1 slots (index 0 unused). */
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();               /* allocation failure */
        cache = new_cache;
        capacity = new_cap;
    }

    /* Initialize base values if this is the first call. */
    if (last < 4) {
        cache[1] = 4;
        cache[2] = 16;
        cache[3] = 60;
        cache[4] = 208;
        last = 4;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Recurrence: a(i) = 6*a(i-1) - 11*a(i-2) + 6*a(i-3) - a(i-4) */
        seq_t term = 6 * cache[i - 1];
        term -= 11 * cache[i - 2];
        term += 6 * cache[i - 3];
        term -= cache[i - 4];
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
