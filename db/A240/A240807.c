/*
 * A240807  a(0)=a(1)=-1, a(2)=2; thereafter a(n) =
 *          a(n-1-a(n-1))+a(n-2-a(n-2)) unless a(n-1) <= n-1 or a(n-2) <= n-2
 *          in which case the sequence terminates.
 * https://oeis.org/A240807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=a(1)=-1, a(2)=2; for n>2: a(n)=a(n-1-a(n-1))+a(n-2-a(n-2))
 * Range:     i8 n <= 259, i16 n <= 9715, i32 n <= 9715, i64 n <= 9715,
 *            i128 n <= 9715, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;

        /* initialise base terms (idempotent on reallocation) */
        if (new_size > 0)  cache[0] = (seq_t)-1;
        if (new_size > 1)  cache[1] = (seq_t)-1;
        if (new_size > 2)  cache[2] = (seq_t)2;

        idx_t start = (cache_size < 3) ? 3 : cache_size;
        for (idx_t i = start; i <= n; ++i) {
            /* compute indices via signed arithmetic in seq_t */
            seq_t idx1_val = (seq_t)(i - 1) - cache[i-1];
            seq_t idx2_val = (seq_t)(i - 2) - cache[i-2];
            idx_t idx1 = (idx_t)idx1_val;
            idx_t idx2 = (idx_t)idx2_val;
            cache[i] = cache[idx1] + cache[idx2];
        }
        cache_size = new_size;
    }
    return cache[n];
}
