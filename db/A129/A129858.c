/*
 * A129858  A triangle of coefficients based on A000217: a(n)=Binomial[n+2,2];
 *          t(n,m)=a(n - m + 1)*a(m + 1) - a((n - m + 1)*(m + 1)).
 * https://oeis.org/A129858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   t(r,c)=a(r‑c+1)*a(c+1)-a((r‑c+1)*(c+1)), a(k)=binom(k+2,2)
 *            Sequence a(n) is the triangle t(r,c) read by rows (row 0 has 1
 *            element, row r has r+1 elements)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   tri_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/tri_a.h"

seq_t a(idx_t n)
{
    /* find 0‑based row r such that (r + 1)(r + 2)/2 ≥ n */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 < n)
        ++r;

    /* linear index of the first element of this row (1‑based) */
    idx_t row_start = r * (r + 1) / 2 + 1;

    /* column index within the row (0‑based) */
    idx_t m = n - row_start;            /* 0 ≤ m ≤ r */

    /* arguments for the a‑function */
    idx_t k1 = r - m + 1;               /* (r‑m+1) */
    idx_t k2 = m + 1;                   /* (m+1) */
    idx_t mul = k1 * k2;                /* (r‑m+1)*(m+1) */

    /* compute the triangle entry */
    seq_t a1 = tri_a(k1);
    seq_t a2 = tri_a(k2);
    seq_t prod = a1 * a2;
    seq_t a3 = tri_a(mul);

    return prod - a3;
}
