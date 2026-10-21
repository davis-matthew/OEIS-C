/*
 * A219605  Square array T(n,k), read by antidiagonals: T(n,2*k) =
 *          T(n,2*k-1)*n, T(n,2*k+1) = T(n,2*k)+n, T(n,0) = 1.
 * https://oeis.org/A219605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A219605 – square array read by antidiagonals.
    T(n,0) = 1
    T(n,2k)   = T(n,2k-1) * n
    T(n,2k+1) = T(n,2k)   + n
    a(n) is the entry with coordinates (i,j) where i+j equals the
    running diagonal index, enumerated as
      (0,0),(0,1),(1,0),(0,2),(1,1),(2,0),…
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    /* Row (i) and column (j) within that diagonal. */
    idx_t start = d * (d + 1) / 2;  /* first index of the diagonal */
    idx_t i = n - start;            /* row index   (0 .. d)          */
    idx_t j = d - i;                /* column index (0 .. d)          */

    /* Compute T(i,j) directly from the recurrence; j is the column. */
    seq_t val = (seq_t)1;           /* T(i,0) = 1 */

    for (idx_t k = 1; k <= j; ++k) {
        if (k & 1)                  /* odd k -> T(i,2k-1)+i */
            val = val + (seq_t)i;
        else                        /* even k -> T(i,2k)*i  */
            val = val * (seq_t)i;
    }

    return val;
}
