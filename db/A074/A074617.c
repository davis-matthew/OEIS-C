/*
 * A074617  a(n) = 5^n + 8^n.
 * https://oeis.org/A074617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^n + 8^n; recurrence: a(n) = 13*a(n-1) - 40*a(n-2),
 *            a(0)=2, a(1)=13
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow8 = big_pow((seq_t)8, (seq_t)n);
    return pow5 + pow8;
}
