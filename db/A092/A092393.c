/*
 * A092393  Triangle read by rows: T(n,k) = (n+k)*binomial(n,k) (for
 *          k=0..n-1).
 * https://oeis.org/A092393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r + k) * binomial(r, k) where r is the smallest integer
 *            with r(r+1)/2 ≥ n and k = n - (r-1)r/2 - 1
 * Range:     u8 n <= 24, u16 n <= 98, u32 n <= 448, u64 n <= 1859,
 *            u128 n <= 7807, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, but safe guard */

    /* Find the row r such that the cumulative number of terms up to that
       row (r(r+1)/2) is at least n.  For n up to 10 000 the loop is
       trivial (r ≈ 140). */
    idx_t r   = 1;
    idx_t cum = 1;                        /* cumulative count up to current row */

    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Number of terms before this row. */
    idx_t prev = cum - r;                 /* (r-1)*r/2 */

    /* Column index within the row (0‑based). */
    idx_t k = n - prev - 1;

    /* Compute the triangle entry.  binomial returns a seq_t, so cast
       r and k to seq_t before multiplication. */
    seq_t bin = binomial(r, k);
    seq_t term = ((seq_t)r + (seq_t)k) * bin;
    return term;
}
