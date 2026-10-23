/*
 * A174719  Triangle T(n, k, q) = (1-q^n)*( binomial(n, k) - 1 ) + 1, with q =
 *          3, read by rows.
 * https://oeis.org/A174719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (1 - 3^r) * (C(r,k) - 1) + 1, where r and k satisfy * n =
 *            r*(r+1)/2 + k (0 ≤ k ≤ r). The triangle is read row by row
 * Range:     i8 n <= 10, i16 n <= 29, i32 n <= 96, i64 n <= 360,
 *            i128 n <= 1346, big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row index r such that the cumulative count of entries
     * up to row r is > n.  The cumulative count up to row m is
     * sum_{i=0}^{m} (i+1) = (m+1)(m+2)/2. */
    idx_t low = 0;
    idx_t high = n;   /* high is an upper bound where the sum surely exceeds n */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t sum = (mid + 1) * (mid + 2) / 2;
        if (sum > n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t r = low;                       /* row index */
    idx_t prev_sum = r * (r + 1) / 2;    /* first linear index of this row */
    idx_t k = n - prev_sum;              /* column index within the row */

    seq_t pow3_r = pow3(r);              /* 3^r */
    seq_t comb   = binom(r, k);          /* C(r,k) */

    /* T(r,k,3) = (1 - 3^r) * (C(r,k) - 1) + 1 */
    seq_t term = 1 - (pow3_r - 1) * (comb - 1);
    return term;
}
