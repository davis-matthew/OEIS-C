/*
 * A176991  Triangle t(n,m) = binomial(n+m,m) - binomial(n-m,m), 1<=m<=n, read
 *          by rows.
 * https://oeis.org/A176991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r + k, k) - binomial(r - k, k) * where r is the
 *            smallest integer with r(r+1)/2 ≥ n, * and k = n - (r-1)r/2 (the
 *            position inside that row)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row number r. */
    idx_t r = 1;
    idx_t cum = 1;                 /* total elements up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Position k within the row (1‑based). */
    idx_t k = n - (cum - r);       /* because cum - r = sum of rows 1..r-1 */

    /* Compute the triangle entry and return it. */
    seq_t t1 = binom(r + k, k);    /* C(r+k, k) */
    seq_t t2 = binom(r - k, k);    /* C(r-k, k), zero if r-k < k */
    return t1 - t2;
}
