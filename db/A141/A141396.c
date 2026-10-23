/*
 * A141396  Triangle read by rows, antidiagonals of a multiplication table:
 *          3^n * (numbers not multiples of 3).
 * https://oeis.org/A141396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3^i * (3*floor(j/2)+1+(j%2)), where d = floor((sqrt(8*n +
 *            1) - 1) / 2), i = n - d*(d+1)/2, j = d - i
 * Range:     u8 n <= 24, u16 n <= 74, u32 n <= 249, u64 n <= 900,
 *            u128 n <= 3401, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find the antidiagonal index d by solving d(d+1)/2 <= n < (d+1)(d+2)/2. */
    double dn = (double)n;
    idx_t d = (idx_t)((sqrt(8.0 * dn + 1.0) - 1.0) * 0.5L);

    /* Linear index where this antidiagonal starts. */
    idx_t start = d * (d + 1) / 2;

    /* Position within the antidiagonal. */
    idx_t offset = n - start;

    /* Row and column indices. */
    idx_t i = offset;          /* row */
    idx_t j = d - offset;      /* column */

    /* Compute 3^i in seq_t. */
    seq_t pow3i = 1;
    for (idx_t k = 0; k < i; ++k)
        pow3i *= (seq_t)3;

    /* Compute the j‑th number that is not a multiple of 3. */
    idx_t q = j >> 1;                 /* floor(j/2) */
    seq_t bj = (seq_t)(3 * q + ((j & 1) ? 2 : 1));

    /* Result: 3^i * bj. */
    return pow3i * bj;
}
