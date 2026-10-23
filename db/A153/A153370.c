/*
 * A153370  Number of zig-zag paths from top to bottom of a rectangle of width
 *          11 with n rows whose color is not that of the top right corner.
 * https://oeis.org/A153370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-2) - 9*a(n-4) + 2*a(n-6) Initial terms: a(1)=5,
 *            a(2)=10, a(3)=18, a(4)=36, a(5)=66, a(6)=132
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) return 0;

    if (n >= cache_size) {
        /* expand cache to hold up to n */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n+1) * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */

        /* initialise new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 1)          new_cache[i] = (seq_t)5;
            else if (i == 2)     new_cache[i] = (seq_t)10;
            else if (i == 3)     new_cache[i] = (seq_t)18;
            else if (i == 4)     new_cache[i] = (seq_t)36;
            else if (i == 5)     new_cache[i] = (seq_t)66;
            else if (i == 6)     new_cache[i] = (seq_t)132;
            else /* i >= 7 */ {
                new_cache[i] =
                    (seq_t)6 * new_cache[i-2]
                  - (seq_t)9 * new_cache[i-4]
                  + (seq_t)2 * new_cache[i-6];
            }
        }

        cache = new_cache;
        cache_size = n + 1;
    }

    return cache[n];
}
