/*
 * A130671  Triangular sequence based on Pascal's triangle: t(n,m) =
 *          2*binomial(m, n) - (1 + n*(m - n)).
 * https://oeis.org/A130671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(k,m) with t(n,m) = 2*binomial(m,n) - (1 + n*(m-n))
 *            where (m,k) are the coordinates of the n‑th element in the
 *            triangle read in row-major order (offset 1)
 * Range:     u8 n <= 40, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes the shared binomial routine */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row m (0‑based) that contains the index n.
       cum is the cumulative count of terms up to row m. */
    idx_t m = 0;
    idx_t cum = 1;                     /* terms in row 0 */

    while (cum < n) {
        ++m;
        cum += m + 1;                  /* add row m terms */
    }

    /* First index of row m (1‑based). */
    idx_t offset = (m * (m + 1)) / 2 + 1;

    /* Position within the row (0‑based). */
    idx_t k = n - offset;              /* 0 ≤ k ≤ m */

    /* Compute t(k,m) = 2*binomial(m,k) - (1 + k*(m-k)). */
    seq_t bin = binomial(m, k);         /* binomial coefficient in seq_t */
    seq_t term = 2 * bin;               /* 2*binomial */

    seq_t adjust = (seq_t)k * (seq_t)(m - k) + 1;   /* 1 + k*(m-k) */
    term -= adjust;

    return term;
}
