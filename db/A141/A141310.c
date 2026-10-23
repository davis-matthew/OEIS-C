/*
 * A141310  The odd numbers interlaced with the constant-2 sequence.
 * https://oeis.org/A141310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 2k+1 (odd numbers), a(2k+1) = 2
 * Range:     u8 n <= 255, u16 n <= 8133, u32 n <= 8133, u64 n <= 8133,
 *            u128 n <= 8133, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n & 1U)          /* odd index */
        return (seq_t)2;
    return (seq_t)n + (seq_t)1;   /* even index: n+1 */
}
