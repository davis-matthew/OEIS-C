/*
 * A047401  Numbers that are congruent to {0, 1, 3, 6} mod 8.
 * https://oeis.org/A047401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + d[(n-1)%4] with d = {0,1,3,6}
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t d[4] = {0, 1, 3, 6};
    idx_t k = (n - 1) / 4;          /* block number */
    idx_t r = (n - 1) % 4;          /* position inside block */
    return (seq_t)8 * (seq_t)k + d[(size_t)r];
}
