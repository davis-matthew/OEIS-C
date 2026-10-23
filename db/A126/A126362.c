/*
 * A126362  Number of base 8 n-digit numbers with adjacent digits differing by
 *          one or less.
 * https://oeis.org/A126362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) - a(n-3) + 2*a(n-4) for n>4 Initial
 *            values: a(0)=1, a(1)=8, a(2)=22, a(3)=62, a(4)=176
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;   /* number of cached elements */

    if (n >= cached_size) {
        idx_t new_size = n + 1;                      /* allocate up to n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – terminate the program */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        for (idx_t i = cached_size; i <= n; ++i) {
            if (i == 0)        cache[i] = 1;
            else if (i == 1)   cache[i] = 8;
            else if (i == 2)   cache[i] = 22;
            else if (i == 3)   cache[i] = 62;
            else if (i == 4)   cache[i] = 176;
            else
                cache[i] = 5 * cache[i-1] - 6 * cache[i-2]
                                 - cache[i-3] + 2 * cache[i-4];
        }
        cached_size = new_size;
    }
    return cache[n];
}
