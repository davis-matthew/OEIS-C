/*
 * A134390  Duplicate of A131416.
 * https://oeis.org/A134390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = entry (i,j) of the lower‑triangular matrix M = (A000012 *
 *            A002260 + A002260 * A000012) – A000012, enumerated row‑major
 *            where A000012 is all ones and A002260 has entries equal to the
 *            column index. The resulting value is given by: M(i,j) = j*i – j²
 *            + j + (i² + i)/2 – (j² – j)/2 – 1 for i ≥ j...
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the row i (1‑based) containing position n. */
    idx_t i = 1;                    /* current row number      */
    idx_t t = 1;                    /* t = i*(i+1)/2: current triangular sum */
    while ((idx_t)t < n) {
        ++i;
        t += i;
    }

    /* Determine column j: position within the row. */
    idx_t prev_sum = t - i;         /* t(i-1) */
    idx_t j = n - prev_sum;         /* 1‑based column index */

    /* Convert indices to seq_t for computation. */
    seq_t si = (seq_t)i;
    seq_t sj = (seq_t)j;

    /* Compute the formula. */
    seq_t term1 = sj * si - sj * sj + sj;
    seq_t term2 = (si * si + si) / 2;
    seq_t term3 = (sj * sj - sj) / 2;

    return term1 + term2 - term3 - 1;
}
