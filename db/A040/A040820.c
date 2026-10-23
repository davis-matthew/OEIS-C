/*
 * A040820  Continued fraction for sqrt(850).
 * https://oeis.org/A040820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=29; for n>0, the terms repeat 6,2,6,58 with period 4
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;
    static const seq_t period[4] = {(seq_t)6, (seq_t)2, (seq_t)6, (seq_t)58};
    return period[(size_t)((n - 1) % 4)];
}
