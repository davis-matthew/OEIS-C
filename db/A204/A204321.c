/*
 * A204321  Start with 4. Square the previous term and subtract it.
 * https://oeis.org/A204321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)*(a(n-1)-1),  a(1) = 4 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t used  = 0;

    if (n == 0) return 0;                /* undefined, offset is 1 */

    /* Allocate or grow cache to at least n+1 elements (index 1‑based) */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cap + 1));
        if (!cache) return 0;           /* allocation failure (unlikely) */
    }
    if ((size_t)n > cap) {
        size_t newcap = cap ? cap * 2 : 1;
        while ((size_t)n > newcap) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (newcap + 1));
        if (!tmp) return 0;
        cache = tmp;
        cap   = newcap;
    }

    /* Compute terms up to n if they have not been computed yet */
    if ((size_t)n > used) {
        if (used == 0) {
            cache[1] = (seq_t)4;
            used = 1;
        }
        for (size_t i = used + 1; i <= (size_t)n; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = prev * (prev - 1);
        }
        used = n;
    }

    return cache[(size_t)n];
}
