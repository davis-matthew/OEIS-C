/*
 * A062264  Coefficient triangle of certain polynomials N(4; m,x).
 * https://oeis.org/A062264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(m,k) * C(m+4,k) where m is the row index satisfying
 *            m(m+1)/2 <= n < (m+1)(m+2)/2 and k = n - m(m+1)/2 is the column
 *            index within that row
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 160, u64 n <= 577,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i); /* multiply by next numerator term */
        res = res / i;           /* exact division */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row m such that m(m+1)/2 <= n < (m+1)(m+2)/2. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n) {
        ++m;
    }
    /* m is now the correct row index */
    idx_t start = m * (m + 1) / 2;   /* first index of row m */
    idx_t k = n - start;            /* column within the row */

    seq_t val1 = binomial_small(m, k);        /* C(m,k) */
    seq_t val2 = binomial_small(m + 4, k);    /* C(m+4,k) */
    return val1 * val2;
}
