/*
 * A135352  Period 5: repeat [1,2,2,1,3].
 * https://oeis.org/A135352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [1,2,2,1,3]_{(n-1) mod 5}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pattern[5] = {1, 2, 2, 1, 3};
    /* n is >=1 by definition of offset, so (n-1) is safe. */
    return pattern[(size_t)((n - 1) % 5)];
}
