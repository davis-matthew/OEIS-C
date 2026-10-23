/*
 * A186941  a(n)=2^C(n,2)*(2(4^n-n*2^n)-1).
 * https://oeis.org/A186941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^C(n,2) * (2*(4^n - n*2^n) - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C(n,2) is the binomial coefficient n choose 2 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* exponent for 2^C(n,2) */
    idx_t c_exp = (idx_t)n * ((idx_t)n - 1) / 2;
    seq_t powC = pow2(c_exp);          /* 2^C(n,2) */

    /* compute 4^n = 2^(2n) and 2^n */
    idx_t exp4 = 2 * n;
    seq_t pow4n = pow2(exp4);          /* 4^n */
    seq_t pow2n = pow2(n);             /* 2^n */

    /* diff = 4^n - n*2^n */
    seq_t diff = pow4n - pow2n * (seq_t)n;

    /* inner = 2*diff - 1 = 2*(4^n - n*2^n) - 1 */
    seq_t inner = diff * 2;
    inner -= 1;

    /* final result */
    return powC * inner;
}
