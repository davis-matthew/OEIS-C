/*
 * A144744  Recurrence sequence a(n)=a(n-1)^2-a(n-1)-1, a(0)=4.
 * https://oeis.org/A144744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 - a(n-1) - 1, a(0) = 4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t len = 0;   /* number of stored terms (len > 0 => cache[0] is a(0)) */

    /* Ensure we can produce term n */
    if ((size_t)n >= len) {
        /* Determine new capacity (at least len + 1) */
        size_t need = (size_t)n + 1;
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < need) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap = new_cap;

        /* If this is the first call, initialize a(0) */
        if (len == 0) {
            cache[0] = (seq_t)4;
            len = 1;
        }

        /* Compute missing terms up to n */
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            seq_t prev = cache[i-1];
            cache[i] = prev * prev - prev - 1;
        }

        len = (size_t)n + 1;
    }

    return cache[n];
}
