/*
 * A086348  On a 3 X 3 board, number of n-move routes of chess king ending in
 *          the central square.
 * https://oeis.org/A086348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 12*a(n-2) + 8*a(n-3), a(0) = 1, a(1) = 8, a(2)
 *            = 32
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of computed values */
    static idx_t  size  = 0;      /* number of entries currently stored */

    /* on first call, allocate one slot */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure – unlikely in tests */
        size = 0;
    }

    /* if we need more terms, extend the cache and compute them */
    if ((idx_t)n >= size) {
        size_t newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* allocation failure – unlikely */
        cache = tmp;

        /* compute from the first missing term up to n */
        for (idx_t i = size; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)1;
            else if (i == 1)
                cache[i] = (seq_t)8;
            else if (i == 2)
                cache[i] = (seq_t)32;
            else
                cache[i] = (seq_t)(2 * cache[i - 1] + 12 * cache[i - 2] + 8 * cache[i - 3]);
        }
        size = (idx_t)n + 1;
    }
    return cache[n];
}
