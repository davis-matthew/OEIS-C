/*
 * A202672  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of the symmetric matrix
 *          A087062 based on (1,1,1,1,...); by antidiagonals.
 * https://oeis.org/A202672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 57, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (-1)^k * C(2*r - k, k)
 *
 *  r   = smallest integer such that r*(r+3)/2 >= n   (row number of the
 *  coefficient in the flattened array)
 *
 *  k   = n - r*(r+1)/2                             (exponent of x in
 *  the characteristic polynomial)
 *
 *  C(n,k) denotes the binomial coefficient.  The sign is (-1)^k.
 */

#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but guard against accidental call */
        return 0;

    /* find the row r where the n-th term lies */
    idx_t r = 1;
    idx_t cum = 0;          /* cumulative number of terms up to current row */
    while (1) {
        cum += r + 1;       /* each row r has r+1 terms */
        if (cum >= n)
            break;
        ++r;
    }

    /* position within the row (1‑based) */
    idx_t prev_cum = cum - (r + 1);   /* terms before current row */
    idx_t pos = n - prev_cum;         /* 1 … r+1 */
    idx_t k = pos - 1;                /* coefficient index (0‑based) */

    /* compute the absolute coefficient: C(2*r - k, k) */
    seq_t coeff = binomial((idx_t)(2 * r - k), k);

    /* apply the sign (-1)^k */
    if (k & 1)
        coeff = -coeff;

    return coeff;
}
