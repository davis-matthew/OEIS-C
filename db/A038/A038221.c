/*
 * A038221  Triangle whose (i,j)-th entry is binomial(i,j)*3^(i-j)*3^j.
 * https://oeis.org/A038221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r, j) * 3^r where r = floor((sqrt(1 + 8*n) - 1)
 *            / 2) j = n - r*(r+1)/2 (index within row r, 0 ≤ j ≤ r) This
 *            flattens the triangle T(i,j) = binomial(i,j)*3^i row‑by‑row
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 108, u64 n <= 361,
 *            u128 n <= 1348, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>                      /* sqrt()  */
#include "../utils/pow3.h"             /* seq_t pow3(idx_t exp); */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    double  d   = sqrt((double)(1 + 8 * (double)n));   /* 1 + 8n fits in 64‑bit for all reasonable n */
    idx_t   r   = (idx_t)((d - 1.0) / 2.0);             /* floor by truncation */

    /* Index within the row */
    idx_t   prev_sum = r * (r + 1) / 2;                 /* sum of entries in rows < r */
    idx_t   j        = n - prev_sum;                    /* 0 ≤ j ≤ r */

    /* Use symmetry to reduce loop length */
    if (j > r - j)
        j = r - j;

    /* Compute binomial(r, j) multiplicatively */
    seq_t binom = 1;
    for (idx_t k = 1; k <= j; ++k) {
        /* (r - j + k) is the numerator factor for step k */
        binom = (binom * (seq_t)(r - j + k)) / (seq_t)k;
    }

    /* Multiply by 3^r */
    return binom * pow3(r);
}
