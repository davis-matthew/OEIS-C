/*
 * A013991  Number of edge-disjoint paths between opposite corners of a 2 X n
 *          grid.
 * https://oeis.org/A013991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 4, a(3) = 16, a(4) = 72, a(5) = 335,
 *            a(6) = 1562 Recurrence: a(n) = 6*a(n-1) - 6*a(n-2) - 4*a(n-3) +
 *            16*a(n-4) - 16*a(n-5) + 7*a(n-6) (n ≥ 7)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t max_cached = (idx_t)-1;     /* no values cached yet */

    /* lazy allocation of the cache */
    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* enlarge cache if needed */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – fall back to direct recurrence. */
            /* The following values are the first seven terms. */
            static const seq_t init[7] = { 1, 1, 4, 16, 72, 335, 1562 };
            if (n < 7) return init[n];
            seq_t a6 = init[6], a5 = init[5], a4 = init[4],
                  a3 = init[3], a2 = init[2], a1 = init[1];
            seq_t ai;
            for (idx_t i = 7; i <= n; ++i) {
                ai = 6 * a1 - 6 * a2 - 4 * a3 + 16 * a4 - 16 * a5 + 7 * a6;
                a6 = a5; a5 = a4; a4 = a3; a3 = a2; a2 = a1; a1 = ai;
            }
            return ai;
        }
        cache = tmp;
        capacity = new_cap;
    }

    /* initialise the cache with the first seven terms if not yet done */
    if (max_cached == (idx_t)-1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)72;
        cache[5] = (seq_t)335;
        cache[6] = (seq_t)1562;
        max_cached = 6;
    }

    /* if the requested index is already computed, just return it */
    if (n <= max_cached)
        return cache[n];

    /* compute subsequent terms up to n using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = 6 * cache[i-1]
                 - 6 * cache[i-2]
                 - 4 * cache[i-3]
                 + 16 * cache[i-4]
                 - 16 * cache[i-5]
                 + 7  * cache[i-6];
    }
    max_cached = n;
    return cache[n];
}
