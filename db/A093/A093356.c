/*
 * A093356  Number of occurrences of pattern 1-2 after n iterations of
 *          morphism A007413.
 * https://oeis.org/A093356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*4^(n-1) + 2^n)/2 for n>=2, a(1)=3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)3;

    idx_t exp4 = 2 * (n - 1);   /* 4^(n‑1) == 2^(2*(n-1)) */
    seq_t pow4 = pow2(exp4);
    seq_t pow2n = pow2(n);

    seq_t num = (seq_t)3 * pow4 + pow2n; /* numerator */
    return num / 2;
}
