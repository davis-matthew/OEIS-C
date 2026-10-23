/*
 * A168575  a(n) = (10^n + 1)^3.
 * https://oeis.org/A168575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10^n + 1)^3
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t ten_pow = pow10(n);          /* 10^n */
    seq_t val     = ten_pow + (seq_t)1;/* 10^n + 1 */
    seq_t res     = val * val * val;   /* (10^n + 1)^3 */
    return res;
}
