/*
 * A024120  a(n) = 10^n - n^6.
 * https://oeis.org/A024120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - n^6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t pow10_n = pow10(n);
    seq_t n_seq   = (seq_t)n;
    seq_t n6      = 1;
    for (idx_t i = 0; i < 6; ++i)
        n6 *= n_seq;
    return pow10_n - n6;
}
