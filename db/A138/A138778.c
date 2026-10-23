/*
 * A138778  Triangle read by rows: T(n,k)=k*binomial(n-2k,3k) (n>=5,
 *          1<=k<=n/5).
 * https://oeis.org/A138778
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = k * binomial(n-2k, 3k) (n ≥ 5, 1 ≤ k ≤ ⌊n/5⌋) Sequence
 *            read by rows, offset 5. a(5) is the first term
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* undefined for n < 5; caller may ignore the result */
    if (n < 5)
        return 0;

    /* m is the position in the row‑by‑row enumeration  (a(5) → m=1) */
    idx_t m = n - 4;

    /* Find the row that contains the m‑th term. */
    idx_t row = 5;
    idx_t cum = 0;                 /* total terms before the current row */

    while (1) {
        idx_t row_terms = row / 5;     /* ⌊row/5⌋ */
        if (cum + row_terms >= m)
            break;
        cum += row_terms;
        ++row;
    }

    /* Position k within this row (1‑based). */
    idx_t k = m - cum;                /* 1 ≤ k ≤ ⌊row/5⌋ */

    /* Compute T(row, k) = k * binomial(row - 2k, 3k). */
    idx_t n1 = row - 2 * k;           /* n-2k */
    idx_t r1 = 3 * k;                 /* 3k */

    seq_t bin = binom(n1, r1);
    return ((seq_t)k) * bin;
}
