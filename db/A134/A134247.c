/*
 * A134247  A007318 * triangle by rows, n-th row = (n-1) zeros followed by
 *          T(n), 1.
 * https://oeis.org/A134247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(r, k) + C(r, k+1) * ( (k+1)*(k+2)/2 ) where r = floor(
 *            (sqrt(8*n+1)-1)/2 ) is the row of the flattened
 *            binomial‑transformed triangle whose rows are S[i][j] = C(i,j) +
 *            C(i,j+1)*T(j+1) (T(m)=m(m+1)/2), and k = n - r*(r+1)/2 is the
 *            column within that row
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binomial coefficient C(n, k) using seq_t */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;        /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;        /* division is exact */
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2) ) / 2 <= n ) {
        ++r;
    }

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;            /* column within row r */

    seq_t ans = binom(r, k);         /* first term */

    if (k < r)                       /* second term exists */
    {
        seq_t tri = (seq_t)(k + 1) * (seq_t)(k + 2) / 2;          /* T(k+1) */
        ans += binom(r, k + 1) * tri;
    }

    return ans;
}
