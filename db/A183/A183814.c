/*
 * A183814  1/81 the number of (n+1) X 4 0..2 arrays with no 2 X 2 subblock
 *          being a reflection across the shared element pair of any
 *          horizontal or vertical neighbor.
 * https://oeis.org/A183814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*a(n-1) + 560*a(n-2) - 10054*a(n-3) + 24480*a(n-4) +
 *            45920*a(n-5) a(1) = 64 a(2) = 3256 a(3) = 163208 a(4) = 8231592
 *            a(5) = 414246824
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cap = 0;   /* number of terms stored */

    size_t needed = (size_t)n;
    if (needed > cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache && needed != 0)   /* allocation failed */
            return (seq_t)0;
        cache = new_cache;

        for (size_t i = cap + 1; i <= needed; ++i) {
            if (i == 1)
                cache[i - 1] = (seq_t)64;
            else if (i == 2)
                cache[i - 1] = (seq_t)3256;
            else if (i == 3)
                cache[i - 1] = (seq_t)163208;
            else if (i == 4)
                cache[i - 1] = (seq_t)8231592;
            else if (i == 5)
                cache[i - 1] = (seq_t)414246824;
            else {
                seq_t t = cache[i - 2] * (seq_t)43
                        + cache[i - 3] * (seq_t)560
                        - cache[i - 4] * (seq_t)10054
                        + cache[i - 5] * (seq_t)24480
                        + cache[i - 6] * (seq_t)45920;
                cache[i - 1] = t;
            }
        }
        cap = needed;
    }

    return cache[n - 1];
}
