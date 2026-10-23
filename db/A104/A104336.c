/*
 * A104336  Weight distribution of [24,12,8] binary Golay code.
 * https://oeis.org/A104336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=1, a(8)=a(16)=759, a(12)=2576, a(24)=1, otherwise 0 (weight
 *            distribution of the binary [24,12,8] Golay code)
 * Range:     u8 n <= 7, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0 || n == 24)
        return (seq_t)1;
    if (n == 8 || n == 16)
        return (seq_t)759;
    if (n == 12)
        return (seq_t)2576;
    return (seq_t)0;
}
