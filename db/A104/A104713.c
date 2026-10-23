/*
 * A104713  Triangle T(n,k) = binomial(n,k), read by rows, 3 <= k <=n .
 * https://oeis.org/A104713
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r, k) where r and k are determined by the * n-th
 *            term of the triangle of binomial coefficients C(r, k) with * 3 ≤
 *            k ≤ r, read by rows. The sequence is 1‑indexed with a(3)=1
 * Range:     u8 n <= 39, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* offset does not matter for n < 3; return 0 */
    if (n < 3)
        return (seq_t)0;

    /* m is the position (1‑based) inside the enumeration of the triangle. */
    idx_t m = n - 2;   /* a(3) → m = 1, a(4) → m = 2, … */

    /* Find the row r such that the cumulative number of terms up to
     * row r is at least m.  Each row r (r ≥ 3) contributes (r‑2) terms. */
    idx_t r = 3;
    idx_t cumulative = 0;                  /* terms in rows before r */

    while (cumulative + (r - 2) < m) {
        cumulative += (r - 2);
        ++r;
    }

    /* Position within the current row (0‑based). */
    idx_t offset = m - cumulative - 1;
    idx_t k = 3 + offset;   /* k ranges from 3 to r */

    return binomial(r, k);
}
