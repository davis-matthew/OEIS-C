/*
 * A081279  Binomial transform of Chebyshev coefficients A001794.
 * https://oeis.org/A081279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n^3 + 30*n^2 + 103*n + 81) * 3^(n-4) for n>=4 Base
 *            values: a(0)=1, a(1)=8, a(2)=47, a(3)=238
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 72,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)47;
    if (n == 3) return (seq_t)238;

    seq_t nn = (seq_t)n;
    seq_t poly = (seq_t)2 * nn * nn * nn
               + (seq_t)30 * nn * nn
               + (seq_t)103 * nn
               + (seq_t)81;
    seq_t pow3 = pow_seq((seq_t)3, (int)(n - 4));
    return poly * pow3;
}
