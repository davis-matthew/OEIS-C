/*
 * A020720  Pisot sequences E(7,9), P(7,9).
 * https://oeis.org/A020720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) for n >= 3, with initial values a(0)=7,
 *            a(1)=9, a(2)=12
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 72, u64 n <= 150,
 *            u128 n <= 308, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache to handle any order of calls. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last = 2;   /* Highest index computed so far */

    /* Handle the trivial base cases. */
    if (n == 0) return 7;
    if (n == 1) return 9;
    if (n == 2) return 12;

    /* Allocate initial cache if needed. */
    if (cache == NULL) {
        cap = 3;                     /* indices 0..2 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;        /* allocation failure: return 0 silently */
        cache[0] = 7;
        cache[1] = 9;
        cache[2] = 12;
    }

    /* If we need to grow the cache, do so. */
    if ((idx_t)cap <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure: return 0 silently */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i - 2] + cache[i - 3];
    }
    last = n;                          /* update highest computed index */

    return cache[n];
}
