/*
 * A152268  Expansion of g.f. x/(1-7*x+8*x^2).
 * https://oeis.org/A152268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 8*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Static cache for memoization. */
    static seq_t *cache = NULL;  /* Cached values 0..comp-1 */
    static idx_t comp = 0;       /* Number of computed entries. */
    static idx_t cap  = 0;       /* Allocated capacity of 'cache'. */

    /* Initialise cache for the first non‑base call. */
    if (cache == NULL) {
        cap  = 2;
        comp = 2;                         /* 0 and 1 are already known */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure */
        cache[0] = 0;
        cache[1] = 1;
    }

    /* Return if already computed. */
    if (n < comp) return cache[n];

    /* Ensure sufficient capacity. */
    if (cap < n + 1) {
        idx_t new_cap = cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute new entries using the recurrence. */
    for (idx_t k = comp; k <= n; ++k) {
        cache[k] = 7 * cache[k - 1] - 8 * cache[k - 2];
    }
    comp = n + 1;

    return cache[n];
}
