/*
 * A045889  Partial sums of A045618.
 * https://oeis.org/A045889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2+11*n+34)/2 + (n-2)*2^(n+3)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t term1 = (tn * tn + (seq_t)11 * tn + (seq_t)34) / 2;
    seq_t pow = pow2(n + 3);  /* 2^(n+3) */

    if (n < 2) {
        seq_t mult = (seq_t)(2 - n) * pow;  /* (2-n)*2^(n+3) for n=0,1 */
        return term1 - mult;
    } else {
        seq_t mult = (seq_t)(n - 2) * pow;    /* (n-2)*2^(n+3) for n>=2 */
        return term1 + mult;
    }
}
