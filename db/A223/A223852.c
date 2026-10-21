/*
 * A223852  Poly-Cauchy numbers c_5^(-n).
 * https://oeis.org/A223852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..5} Stirling1(5,k)*(k+1)^n * where
 *            Stirling1(5,k) = [0, 24, -50, 35, -10, 1] * therefore a(n) =
 *            24*2^n - 50*3^n + 35*4^n - 10*5^n + 6^n
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset starts at 1 */

    seq_t p2 = pow_uint(2, n); /* 2^n */
    seq_t p3 = pow_uint(3, n); /* 3^n */
    seq_t p4 = pow_uint(4, n); /* 4^n */
    seq_t p5 = pow_uint(5, n); /* 5^n */
    seq_t p6 = pow_uint(6, n); /* 6^n */

    seq_t res = 24 * p2;
    res -= 50 * p3;
    res += 35 * p4;
    res -= 10 * p5;
    res += p6;

    return res;
}
