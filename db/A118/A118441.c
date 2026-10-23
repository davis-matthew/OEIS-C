/*
 * A118441  Triangle L, read by rows, equal to the matrix log of A118435, with
 *          the property that L^2 consists of a single diagonal (two rows down
 *          from the main diagonal).
 * https://oeis.org/A118441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   for n > k, L[n][k] = (n! / k!) * 2^(n-k-1) / (n-k-1)! *
 *            (-1)^(floor((n+1)/2) - floor(k/2) + (n-k)). on the diagonal
 *            L[n][n] = 0. This is the OEIS sequence A118441 flattened
 *            row‑wise
 * Range:     i8 n <= 15, i16 n <= 35, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"   /* factorial(idx_t) */
#include "../utils/pow2.h"        /* pow2(idx_t) */

/* compute the matrix index (row, column) corresponding to the
   flattened index n.  Row r satisfies tri(r) <= n < tri(r+1),
   where tri(r) = r*(r+1)/2.  The following loop is fast because
   r is at most about 140 for n <= 10000. */
static idx_t
row_of_n(idx_t n)
{
    idx_t r = 0;
    idx_t start = 0;                /* start index of current row */
    while ((start + r + 1) <= n) {
        ++r;
        start += r;
    }
    return r;
}

/* return the flattened index of the first element of row r */
static idx_t
tri_start(idx_t r)
{
    /* triangular number tri(r) = r*(r+1)/2 */
    return r * (r + 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_n(n);
    idx_t start = tri_start(r);
    idx_t k = n - start;                /* column index within row r */

    if (r == k)                         /* diagonal entry */
        return (seq_t)0;

    /* magnitude: (r! / k!) * 2^(r-k-1) / (r-k-1)!  */
    idx_t d = r - k;                    /* difference >= 1 */
    seq_t val = factorial(r);
    val /= factorial(k);
    if (d > 1)
        val /= factorial(d - 1);
    val *= pow2(d - 1);

    /* sign: (-1)^(floor((r+1)/2) - floor(k/2) + d) */
    idx_t exp = ((r + 1) / 2) - (k / 2) + d;
    if ((exp & 1) != 0)                 /* odd exponent -> negative sign */
        val = -val;

    return val;
}
