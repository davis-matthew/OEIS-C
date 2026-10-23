/*
 * A171226  a(n) = 9+10^n+9*100^n.
 * https://oeis.org/A171226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9 + 10^n + 9*100^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t t1 = pow10(n);
    seq_t t2 = pow10(n * 2);
    return 9 + t1 + 9 * t2;
}
