/*
 * A099810  a(n) = a(n-1) XOR (a(n-1) + a(n-2)), with a(1)=1, a(2)=3, where
 *          XOR is the binary exclusive OR operation.
 * https://oeis.org/A099810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) XOR (a(n-1)+a(n-2)),   a(1)=1, a(2)=3
 * This implementation memoises previously computed terms.
 * It works for any seq_t type (uint8_t ... unsigned __int128)
 * and any idx_t (uint64_t, unsigned).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;      /* stores terms 1..cache_limit */
    static idx_t cache_limit = 0;    /* last indexed term in cache */

    /* initialise base cases if needed */
    if (cache == NULL) {
        /* allocate space for indices 0..2 (ignore 0) */
        cache = (seq_t *)malloc((3) * sizeof(seq_t));
        if (!cache) return 0;            /* failure (should not happen) */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache_limit = 2;
    }

    /* extend cache up to n */
    if (n > cache_limit) {
        idx_t new_limit = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_limit + 1) * sizeof(seq_t));
        if (!tmp) return 0;  /* failure */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = cache_limit + 1; i <= new_limit; ++i) {
            seq_t prev  = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t sum   = prev + prev2;   /* unsigned addition, wraps naturally */
            cache[i] = prev ^ sum;
        }
        cache_limit = new_limit;
    }

    return cache[n];
}
