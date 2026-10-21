/*
 * A081892  Second binomial transform of C(n+2,2).
 * https://oeis.org/A081892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-2)*(n+2)*(n+9)/2 = 3^n*(n^2+11*n+18)/18
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the two small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Now n >= 2 */
    /* 3^(n-2) */
    seq_t p3 = pow3((idx_t)(n - 2));

    /* Polynomial n^2 + 11*n + 18, guaranteed to be even */
    seq_t sn = (seq_t)n;
    seq_t poly = sn * sn + (seq_t)11 * sn + (seq_t)18;

    /* Divide the even polynomial by 2 first to keep intermediate values small */
    seq_t half_poly = poly >> 1;

    /* Final result */
    return p3 * half_poly;
}
