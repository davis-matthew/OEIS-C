/*
 * A052427  Baxter-Hickerson numbers.
 * https://oeis.org/A052427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*10^(5*n) - 10^(4*n) + 2*10^(3*n) + 10^(2*n) + 10^n +
 *            1)/3
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Precompute powers of 10 */
    seq_t p5 = pow10(5 * n);   /* 10^(5*n) */
    seq_t p4 = pow10(4 * n);   /* 10^(4*n) */
    seq_t p3 = pow10(3 * n);   /* 10^(3*n) */
    seq_t p2 = pow10(2 * n);   /* 10^(2*n) */
    seq_t p1 = pow10(n);       /* 10^n */

    /* Compute numerator: 2*p5 - p4 + 2*p3 + p2 + p1 + 1 */
    seq_t res = p5;
    res += p5;                 /* 2*p5 */
    res -= p4;                 /* -p4 */
    res += p3;
    res += p3;                 /* +2*p3 */
    res += p2;                 /* +p2 */
    res += p1;                 /* +p1 */
    res += (seq_t)1;           /* +1 */

    /* Divide by 3 (exact division) */
    res /= (seq_t)3;

    return res;
}
