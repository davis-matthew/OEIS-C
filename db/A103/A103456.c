/*
 * A103456  a(n) = 0^n + 10^n - 1.
 * https://oeis.org/A103456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0^n + 10^n - 1 (a(0)=1, otherwise a(n)=10^n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t ten_pow = pow10(n);
    return ten_pow - (seq_t)1;
}
