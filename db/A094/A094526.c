/*
 * A094526  Square array of binomial transforms read by antidiagonals.
 * https://oeis.org/A094526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = reading the antidiagonals of T(n,k) with T(n,k)=1+kn +
 *            k(k-1)n(n-1)/2 + k(k-1)(k-2)n(n-1)(n-2)/6 The mapping from a
 *            linear index n to the pair (row,col) is: find m such that
 *            m(m+1)/2 ≤ n < (m+1)(m+2)/2, then let t = n - m(m+1)/2, row = t,
 *            col = m - t. Finally a(n)=T(row,col)
 * Range:     u8 n <= 38, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   _diag_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/_diag_of.h"

seq_t a(idx_t n) {
    idx_t m = _diag_of(n);
    /* cumulative terms up to previous diagonal */
    idx_t prevCum = (idx_t)(((__int128)m * (m + 1)) / 2);
    idx_t t = n - prevCum;          /* offset within the diagonal (0..m) */
    idx_t row = t;                  /* n in T(n,k)   */
    idx_t col = m - t;              /* k in T(n,k)   */

    seq_t nn = (seq_t)row;
    seq_t kk = (seq_t)col;

    seq_t res = 1 + kk * nn;        /* 1 + k*n */

    if (col >= 2 && row >= 2) {
        seq_t term = kk * (kk - 1);
        term *= nn * (nn - 1);
        term /= 2;                  /* /2 for binomial coefficient */
        res += term;
    }
    if (col >= 3 && row >= 3) {
        seq_t term = kk * (kk - 1) * (kk - 2);
        term *= nn * (nn - 1) * (nn - 2);
        term /= 6;                  /* /6 for binomial coefficient */
        res += term;
    }
    return res;
}
