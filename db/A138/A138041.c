/*
 * A138041  a(1) = 1, a(2) = 10; for n>2, a(n+1) = 4*a(n) + 6*a(n-1). Also
 *          a(n) = upper left term in the 2 X 2 matrix [1,3; 3,3].
 * https://oeis.org/A138041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(2) = 10; a(n+1) = 4*a(n) + 6*a(n-1) for n >= 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size = 0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        size_t init = (n >= 2) ? (size_t)n + 1 : 3;
        cache = (seq_t*)malloc(sizeof(seq_t) * init);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)10;
        size = 2;
    }

    /* Extend cache and compute missing values if needed */
    if (n > size) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        for (idx_t i = size + 1; i <= n; ++i) {
            cache[i] = (seq_t)4 * cache[i - 1] + (seq_t)6 * cache[i - 2];
        }
        size = n;
    }

    return cache[n];
}
