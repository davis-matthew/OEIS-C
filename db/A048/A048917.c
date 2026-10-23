/*
 * A048917  Indices of hexagonal numbers which are also 9-gonal.
 * https://oeis.org/A048917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64514*a(n-2) - a(n-4) - 16128, with a(1)=1, a(2)=13,
 *            a(3)=51625, a(4)=822757. This recurrence generates the indices
 *            of hexagonal numbers which are also 9‑gonal
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[1..max_n] stores a(1) .. a(max_n) */
    static idx_t  cache_cap = 0;     /* allocated size (legs are [0..cap-1]) */
    static idx_t  max_computed = 0;  /* largest n with cache[n] defined */

    /* ensure the cache can hold up to index n */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;       /* +1 because we ignore index 0 */
        if (new_cap < 5) new_cap = 5;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise the first four terms once */
    if (max_computed < 4) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)51625;
        cache[4] = (seq_t)822757;
        max_computed = 4;
    }

    /* compute any values that are still missing */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 4) continue;  /* safety for small n */
        seq_t prod = (seq_t)64514 * cache[i - 2];
        cache[i] = prod - cache[i - 4] - (seq_t)16128;
    }
    max_computed = n;

    return cache[n];
}
