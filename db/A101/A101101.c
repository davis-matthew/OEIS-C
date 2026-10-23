/*
 * A101101  a(1)=1, a(2)=5, and a(n)=6 for n >= 3.
 * https://oeis.org/A101101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, uned
 * Formula:   a(1)=1, a(2)=5, a(n)=6 for all n>=3
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)5;
    return (seq_t)6;
}
