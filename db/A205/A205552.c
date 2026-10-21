/*
 * A205552  Square array: C(max(2i-2,j-1),min(2i-2,j-1)), i>=1, j>=1; by
 *          antidiagonals.
 * https://oeis.org/A205552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(max(2i-2, j-1), min(2i-2, j-1)) where (i,j) is
 *            the position of the nth term when the square array
 *            C(max(2i-2,j-1),min(2i-2,j-1)) (i>=1,j>=1) is read by
 *            antidiagonals starting at the northwest corner
 * Range:     u8 n <= 40, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // offset is 1

    /* Find the diagonal d such that d*(d+1)/2 >= n */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) d++;

    /* Number of terms before this diagonal */
    idx_t prev = d * (d - 1) / 2;

    /* Position (0‑based) within the diagonal */
    idx_t pos = n - prev - 1;

    /* Coordinates (i,j) of the term on the anti‑diagonal */
    idx_t i = pos + 1;
    idx_t j = d - pos;

    /* Compute the parameters of the binomial coefficient */
    idx_t t1   = (i << 1) - 2;  /* 2*i - 2 */
    idx_t t2   = j - 1;         /* j - 1 */
    idx_t nmax = t1 > t2 ? t1 : t2;
    idx_t nmin = t1 < t2 ? t1 : t2;

    return binomial(nmax, nmin);
}
