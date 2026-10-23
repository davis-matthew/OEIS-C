/*
 * A127677  Scaled coefficient table for Chebyshev polynomials 2*T(2*n,
 *          sqrt(x)/2) (increasing even scaled powers, without zero entries).
 * https://oeis.org/A127677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the (row, column) entry of the triangle * with
 *            a(r,0)=2*(-1)^r and for m>0 *
 *            a(r,m)=(-1)^(r+m)*r*binomial(r+m-1,2*m-1)/m. * The sequence is
 *            obtained by reading the triangle row‑by‑row * (row 0 first, then
 *            row 1, …) and skipping zeros
 * Range:     i8 n <= 22, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative count up to r-1
     * is <= n < cumulative up to r.  The cumulative count up
     * to row r is (r+1)(r+2)/2.  */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }

    /* Number of elements before this row. */
    idx_t prev = r * (r + 1) / 2;
    idx_t i = n - prev;                /* column index within row; 0 <= i <= r */

    if (i == 0) {                      /* first column */
        seq_t val = 2;
        if (r & 1) val = -val;
        return val;
    }

    /* Compute a(r,i) for i>0 using the binomial coefficient. */
    idx_t a_binom_n = r + i - 1;
    idx_t a_binom_k = 2 * i - 1;
    seq_t bin = binomial(a_binom_n, a_binom_k);

    seq_t val = (seq_t)r * bin;        /* multiply first, then divide by i */
    val /= (seq_t)i;

    if (((r + i) & 1) != 0)            /* adjust the sign */
        val = -val;

    return val;
}
