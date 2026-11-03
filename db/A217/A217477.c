/*
 * A217477  Z-sequence for the Riordan triangle A111125.
 * https://oeis.org/A217477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=3, a(n)=(-1)^n*4*binomial(2*n-1,n) for n>=1
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 16, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 3;
    seq_t res = binomial(2 * n - 1, n);   /* C(2n-1, n) */
    res = res * (seq_t)4;              /* 4*C(2n-1, n) */
    if (n & 1)                         /* (-1)^n */
        res = -res;
    return res;
}
