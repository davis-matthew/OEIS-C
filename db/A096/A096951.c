/*
 * A096951  Sum of odd powers of 2 and of 3 divided by 5.
 * https://oeis.org/A096951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(2*n+1) + 3^(2*n+1)) / 5 Recurrence used: a(0) = 1,
 *            a(1) = 7, a(n) = 13 * a(n-1) - 36 * a(n-2) for n ≥ 2 The
 *            implementation keeps a cache for arbitrary‑order calls
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static idx_t cache_size = 0;
    static seq_t *cache = NULL;

    /*  handle the first two terms directly */
    if (n == 0) return 1;
    if (n == 1) return 7;

    /* grow the cache if required */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                    /* store up to index n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure – undefined behaviour for seq_t */

        /* initialise new entries using the recurrence */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                new_cache[i] = 1;
            } else if (i == 1) {
                new_cache[i] = 7;
            } else {
                /* perform the recurrence with seq_t arithmetic only */
                new_cache[i] = 13 * new_cache[i-1] - 36 * new_cache[i-2];
            }
        }

        cache      = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
