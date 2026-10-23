/*
 * A130615  Sum of the first 10^n 4th powers.
 * https://oeis.org/A130615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            10^{n-1}(10^{n-1}+1)(2*10^{n-1}+1)(3*10^{2(n-1)}+3*10^{n-1}-1)/30
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = sum_{k=1}^{10^{n-1}} k^4 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    if (n == 0) return 0;               /* safety, though not used */
    seq_t m = pow10(n - 1);              /* m = 10^(n-1) */
    seq_t m1 = m + 1;                    /* m + 1 */
    seq_t two_m1 = m + m + 1;            /* 2*m + 1 */
    seq_t m_sq = m * m;                  /* m^2 */
    seq_t ad   = m_sq * 3 + m * 3 - 1;   /* 3*m^2 + 3*m - 1 */
    seq_t prod = m * m1;                 /* m*(m+1) */
    prod = prod * two_m1;                /* * (2m+1) */
    prod = prod * ad;                    /* * (3m^2+3m-1) */
    return prod / 30;                    /* divide by 30 */
}
