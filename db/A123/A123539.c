/*
 * A123539  Irregular triangle t(n,m) = binomial(n-m, n-binomial(m,
 *          floor(m/2))), read by rows, 0 <= m <= n/2.
 * https://oeis.org/A123539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(r-m, r - binomial(m, floor(m/2))), where (r,m)
 *            are obtained from the flattened triangle: row r has floor(r/2)+1
 *            entries
 * Range:     u8 n <= 60, u16 n <= 200, u32 n <= 489, u64 n <= 1527,
 *            u128 n <= 2549, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r and column m corresponding to the flat index n. */
    idx_t r = 0;
    idx_t cum = 0;   /* number of entries before row r */
    while (1) {
        idx_t len = r / 2 + 1;   /* length of the current row */
        if (cum + len > n)
            break;
        cum += len;
        ++r;
    }
    idx_t m = n - cum;   /* 0 <= m <= r/2 */

    /* Compute t(r,m). */
    idx_t first = r - m;
    seq_t binom2 = binomial(m, m / 2);   /* binomial(m, floor(m/2)) */

    /* If binom2 > r the second argument becomes negative → 0. */
    if ((seq_t)r < binom2)
        return 0;

    /* binom2 <= r, safe to convert to idx_t. */
    idx_t binom2i = (idx_t)binom2;
    idx_t second = r - binom2i;

    return binomial(first, second);
}
