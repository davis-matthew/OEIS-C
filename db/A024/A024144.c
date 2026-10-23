/*
 * A024144  a(n) = 12^n - n^4.
 * https://oeis.org/A024144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^n - n^4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow12 = big_pow((seq_t)12, (seq_t)n);

    seq_t nn   = (seq_t)n;
    seq_t n2   = nn * nn;
    seq_t n4   = n2 * n2;

    return pow12 - n4;
}
