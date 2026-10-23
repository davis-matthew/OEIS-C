/*
 * A145599  Triangular array of generalized Narayana numbers: T(n,k) =
 *          5/(n+1)*binomial(n+1,k+4)*binomial(n+1,k-1).
 * https://oeis.org/A145599
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n-th term (n≥4) of the flattened triangle of
 *            generalized Narayana numbers T(r,k) = 5/(r+1) *
 *            binomial(r+1,k+4) * binomial(r+1,k-1) with 1 ≤ k ≤ r-3, r ≥ 4
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------------------------   include utils   -------------------------------- */

#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) → seq_t */
#include "../utils/seq_mul.h"    /* seq_mul(seq_t a, seq_t b) → seq_t */

/* ---------------------------------   implementation   -------------------------------- */

seq_t a(idx_t n) {
    if (n < 4) /* undefined below offset, return 0 */
        return 0;

    /* Convert the index to 0‑based position in the flattened array */
    idx_t idx = n - 4;      /* idx ranges from 0 upwards */

    /* Find t such that t*(t+1)/2 > idx , where t = r-3 (row length). */
    idx_t t = 0;
    while (t * (t + 1) / 2 <= idx)
        ++t;

    /* Row and column in the triangle */
    idx_t r = t + 3;                                /* row number   */
    idx_t k = idx - t * (t - 1) / 2 + 1;            /* column number*/

    /* Compute the generalized Narayana number */
    seq_t bin1 = binomial(r + 1, k + 4);
    seq_t bin2 = binomial(r + 1, k - 1);

    seq_t prod = seq_mul(bin1, bin2);       /* bin1 * bin2   */
    prod = seq_mul(prod, (seq_t)5);         /* *5            */
    prod = prod / (r + 1);                  /* divide by (r+1) */

    return prod;
}
