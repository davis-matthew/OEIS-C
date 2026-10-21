/*
 * A052947  Expansion of 1/(1-x^2-2*x^3).
 * https://oeis.org/A052947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3); a(0) = 1, a(1) = 0, a(2) = 1; for n >=
 *            3 the recurrence holds
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 107,
 *            u128 n <= 213, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialize cache if needed */
    if (cache_size == 0) {
        /* grow to size 3 to hold the first three terms */
        cache_size = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
    }

    /* extend cache up to n if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!cache) /* handle allocation failure */
            return (seq_t)0; /* undefined, but avoid crash */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val_n2 = cache[i-2];
            seq_t val_n3 = cache[i-3];
            cache[i] = val_n2 + (seq_t)2 * val_n3;
        }
        cache_size = new_size;
    }

    return cache[n];
}
