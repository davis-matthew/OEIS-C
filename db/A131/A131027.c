/*
 * A131027  Period 6: repeat [4, 3, 1, 0, 1, 3].
 * https://oeis.org/A131027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) repeats [4, 3, 1, 0, 1, 3] with period 6
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t pattern[6] = {4, 3, 1, 0, 1, 3};
    /* Compute (n-1) mod 6; cast the result to size_t for array indexing. */
    return pattern[(size_t)((n - 1) % 6)];
}
