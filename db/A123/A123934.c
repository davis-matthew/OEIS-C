/*
 * A123934  Triangle T(n,k), 1<=k<=n: forms the odd-indexed trinomial
 *          coefficients (A027907).
 * https://oeis.org/A123934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the flattened triangle of A027907
 *            (odd‑indexed trinomial coefficients). The function finds the row
 *            and column that correspond to the given index n and then sums
 *            the appropriate multinomial terms to obtain the required
 *            coefficient
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that n ≤ r*(r+1)/2. */
    idx_t r = 1;
    while (n > (r * (r + 1)) / 2) {
        ++r;
    }

    /* Determine the column within that row (1‑based). */
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t col = n - prev_tri;          /* 1‑based column index */

    /* Exponent of x in the full (1 + x + x^2)^r expansion. */
    idx_t exp = 2 * col - 1;           /* exp = 2k-1 */

    /* Compute the coefficient: sum over c of C(r, c) * C(r-c, exp-2c). */
    seq_t coeff = 0;
    for (idx_t c = 0; c <= exp / 2 && c <= r; ++c) {
        idx_t b = exp - 2 * c;         /* power of x in the term */
        if (b > r - c) {              /* ensures a = r - (b+c) ≥ 0 */
            continue;
        }
        seq_t term = binom(r, c);
        term *= binom(r - c, b);
        coeff += term;
    }
    return coeff;
}
