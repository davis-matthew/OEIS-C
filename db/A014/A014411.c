/*
 * A014411  Triangular array formed from elements to right of middle of rows
 *          of Pascal's triangle that are not 1.
 * https://oeis.org/A014411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   a(n) = binomial(r, floor(r/2)+1 + (n - cum - 1)) where r is the
 *            smallest integer ≥ 3 such that cum + ((r+1)/2 - 1) ≥ n, cum is
 *            the total number of terms contributed by rows 3 … r-1. This
 *            gives the triangular array of Pascal’s triangle coefficients to
 *            the right of the middle of each row (excluding...
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, undefined for n<1 */

    idx_t r   = 3;                       /* start from the first row that contributes */
    idx_t cum = 0;                       /* cumulative terms before current row */

    while (1) {
        idx_t terms = (r + 1) / 2 - 1;   /* number of terms in row r */
        if (cum + terms >= n) break;    /* nth term lies in row r */
        cum += terms;
        ++r;
    }

    idx_t pos = n - cum - 1;             /* position within row r (0‑based) */
    idx_t k   = r / 2 + 1 + pos;         /* actual column index (k > r/2) */

    return binomial(r, k);
}
