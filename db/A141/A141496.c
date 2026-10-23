/*
 * A141496  a(0)=1; a(1)=5; a(2)=11; a(n)=a(1)*a(n-1).
 * https://oeis.org/A141496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=5; a(2)=11; a(n) = 11*5^(n-2) for n>1 (equivalently
 *            a(n)=5*a(n-1) for n>2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow_uint5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint5.h"

extern seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)11;
    return (seq_t)11 * pow_uint5(n - 2);
}
