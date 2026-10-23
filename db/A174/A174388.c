/*
 * A174388  Triangle T(n, k) = c(n, q)/c(k, q) if k <= floor(n/2), otherwise
 *          c(n, q)/c(n-k, q), where c(n, q) = Product_{j=1..n} (1 - q^j) and
 *          q = 3, read by rows.
 * https://oeis.org/A174388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the flattened triangle T(N,K) where * T(N,K) =
 *            c(N)/c(N-K) if K ≤ floor(N/2) * c(N)/c(K) otherwise, * with c(M)
 *            = ∏_{j=1..M} (1 – 3^j). The flattening is row‑wise * with N =
 *            0,1,2,… and K running from 0 to N
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 38, i64 n <= 59, i128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row (N) and column (K) for the flattened index n. */
    idx_t row = (idx_t)((sqrtl(8.0L * n + 1.0L) - 1.0L) / 2.0L);          /* N */
    idx_t first_in_row = row * (row + 1) / 2;                           /* index of column 0 */
    idx_t col = n - first_in_row;                                       /* K */

    /* Decide which product range to form. */
    idx_t start, end = row;                                              /* inclusive */
    if (col <= row / 2) {                                                /* use c(N-K)  */
        start = row - col + 1;
    } else {                                                             /* use c(K)    */
        start = col + 1;
    }

    /* Compute the product ∏_{j=start..end} (1 – 3^j) . */
    seq_t result = 1;
    seq_t pow3   = 1; /* 3^0 */
    for (idx_t j = 1; j <= end; ++j) {
        pow3 *= 3;                                       /* pow3 = 3^j */
        if (j >= start) {
            result *= (seq_t)(1 - pow3);
        }
    }

    return result;
}
