/*
 * A097738  Pell equation solutions (9*a(n))^2 - 82*b(n)^2 = -1 with
 *          b(n):=A097739(n), n >= 0.
 * https://oeis.org/A097738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 326*a(n-1) - a(n-2), a(0)=1, a(1)=327 (Recurrence derived
 *            from Pell equation (9*a(n))^2 - 82*b(n)^2 = -1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h" /* provides seq_t *alloc_seq_array(size_t n) */

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* allocated size */
    static idx_t max_cached = (idx_t)-1; /* largest index cached */

    /* initialize cache if necessary */
    if (cache == NULL) {
        cap = 2;                          /* store a(0) and a(1) */
        cache = alloc_seq_array(cap);
        cache[0] =  (seq_t)1;
        cache[1] =  (seq_t)327;
        max_cached = 1;
    }

    /* if requested index already cached, return it */
    if (n <= max_cached) {
        return cache[n];
    }

    /* grow cache to hold up to index n */
    if ((idx_t)n+1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (idx_t i = 0; i <= max_cached; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache); /* free old buffer */
        cache = new_cache;
        cap = new_cap;
    }

    /* compute recurrence from the highest cached index up to n */
    for (idx_t i = max_cached; i < n; ++i) {
        /* a(i+1) = 326*a(i) - a(i-1) */
        seq_t term = (seq_t)326 * cache[i] - cache[i-1];
        cache[i+1] = term;
    }

    max_cached = n;
    return cache[n];
}
