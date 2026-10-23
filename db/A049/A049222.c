/*
 * A049222  Number of horizontally convex n-ominoes in which the top row has
 *          exactly 1 square, which is not above the rightmost square in the
 *          second row and the rightmost square in the second row is above the
 *          leftmost square in the third row.
 * https://oeis.org/A049222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 4*a(n-3) with a(1)=0, a(2)=0,
 *            a(3)=0, a(4)=1, a(5)=4 for n >= 6
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_cache = 0;          /* highest index computed */

    if (n == 0) return 0;                /* undefined for n<1, return 0 */

    if (n > max_cache) {
        size_t new_cap = (size_t)(n + 1); /* keep index 0 unused */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }

        for (idx_t i = max_cache + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 3) {
                val = 0;
            } else if (i == 4) {
                val = 1;
            } else if (i == 5) {
                val = 4;
            } else {
                val = 5 * cache[i-1] - 7 * cache[i-2] + 4 * cache[i-3];
            }
            cache[i] = val;
        }
        max_cache = n;
    }
    return cache[n];
}
