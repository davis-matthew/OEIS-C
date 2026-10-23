/*
 * A128235  Triangle read by rows: T(n,k) is the number of sequences of length
 *          n on the alphabet {0,1,2,3}, containing k subsequences 00
 *          (0<=k<=n-1).
 * https://oeis.org/A128235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by flattening the triangle T(n,k), where *
 *            T(n,k) counts sequences of length n over the alphabet {0,1,2,3}
 *            * that contain exactly k subsequences 00. For each linear index
 *            n, * determine the corresponding row r and position k in that
 *            row, then * compute T(r,k) by summing over all...
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 136, u64 n <= 529,
 *            u128 n <= 2146, big any n
 * Helpers:   binom_n_k, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"
#include "../utils/binom_n_k.h"


seq_t a(idx_t n)
{
    /* Map the linear index n to (row, k) in the triangle */
    idx_t row   = 0;   /* current row index */
    idx_t start = 0;   /* index of the first element of the current row */
    idx_t len   = 1;   /* length of the current row: 1 for row 0, else row number */

    while (n >= start + len) {
        start += len;
        ++row;
        len = (row == 0 ? 1 : row);
    }
    idx_t k = n - start;          /* position within the row, 0‑based */
    idx_t m = row;                /* sequence length for T(m,k) */

    /* Compute T(m,k) using the combinatorial formula */
    seq_t result = 0;

    /* R = 0 term (only when k == 0) */
    if (k == 0) {
        result = pow3(m);         /* 3^m */
    }

    /* Sum over R >= 1 */
    idx_t maxR = m > k ? m - k : 0;   /* R cannot exceed m‑k */
    for (idx_t R = 1; R <= maxR; ++R) {
        /* Composition of (k+R) into R parts: C(k+R‑1, R‑1) */
        seq_t comp = binom_n_k(k + R - 1, R - 1);

        /* Positioning of the R runs among the compressed sequence:
           C(m‑k‑R+1, R) (no adjacent runs) */
        seq_t pos = binom_n_k(m - k - R + 1, R);

        /* Remaining non‑zero digits: 3^{m‑k‑R} */
        seq_t pow = pow3(m - k - R);

        /* Term contribution */
        seq_t term = comp;
        term *= pos;
        term *= pow;
        result += term;
    }

    return result;
}
