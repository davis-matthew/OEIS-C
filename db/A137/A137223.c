/*
 * A137223  a(n) = A135574(3*n) + A135574(3*n+1) + A135574(3*n+2).
 * https://oeis.org/A137223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1) + 64*a(n-2) + 64*a(n-3) with initial values
 *            a(0)=3, a(1)=18, a(2)=234
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t  size  = 0;

    /* initialize cache on first call */
    if (size == 0) {
        size = 3;
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory: return 0 */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)18;
        cache[2] = (seq_t)234;
    }

    /* extend cache up to requested index */
    if (n >= size) {
        idx_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory: return 0 */
        cache = tmp;
        /* compute new terms */
        for (idx_t i = size; i < newsize; ++i) {
            seq_t sum = ((seq_t)64) * cache[i-2] + ((seq_t)64) * cache[i-3];
            cache[i] = sum - cache[i-1];
        }
        size = newsize;
    }

    return cache[n];
}
