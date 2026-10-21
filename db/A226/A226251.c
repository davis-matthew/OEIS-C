/*
 * A226251  Period 10: repeat [1, 1, 2, 3, 5, 8, 1, 3, 4, 7].
 * https://oeis.org/A226251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = pattern[((n-1) mod 10)] with pattern =
 *            [1,1,2,3,5,8,1,3,4,7]
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[10] = {1, 1, 2, 3, 5, 8, 1, 3, 4, 7};

    /* a(1) is the first term; behaviour for n < 1 is undefined, but a guard is harmless */
    if (n == 0)
        return (seq_t)0;

    return pattern[(size_t)((n - 1) % 10)];
}
