/*
 * A010706  Period 2: repeat (3,8).
 * https://oeis.org/A010706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + 5*(n mod 2)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t parity = (seq_t)(n & (idx_t)1);
    return (seq_t)3 + (seq_t)5 * parity;
}
