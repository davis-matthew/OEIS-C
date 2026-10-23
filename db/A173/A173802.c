/*
 * A173802  a(n) = (5*10^n - 23)/9.
 * https://oeis.org/A173802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*10^n - 23)/9 (n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    seq_t p10 = pow10(n);                    /* 10^n */
    seq_t t_minus1 = p10 - (seq_t)1;         /* 10^n - 1  (fits in seq_t) */
    seq_t m = t_minus1 / (seq_t)9;           /* (10^n - 1)/9  → integer  */
    seq_t res = m * (seq_t)5;                /* 5 * m  */
    res -= (seq_t)2;                         /* subtract 2  */
    return res;
}
