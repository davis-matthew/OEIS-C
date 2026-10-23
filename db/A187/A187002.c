/*
 * A187002  Triangle T(n,m): column m is the expansion of
 *          (x+2*x^2+3*x^3+3*x^4+2*x^5+x^6)^m.
 * https://oeis.org/A187002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle T with offset 1, * where
 *            T(r,c) = Σ_{k=c}^{r} C(c,k−c) · Σ_{j=0}^{k} C(k,j) · C(j,
 *            r−3k+2j). * The linear index n is decoded to row r and column c
 *            by * r(r−1)/2 < n ≤ r(r+1)/2, c = n − r(r−1)/2
 * Range:     u8 n <= 31, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b); */
 
seq_t a(idx_t n)
{
    /* Decode linear index to row r and column c of the triangle. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;
    idx_t prev_total = r * (r - 1) / 2;
    idx_t c = n - prev_total;   /* 1 ≤ c ≤ r */
 
    /* Compute T(r,c) according to the closed‑form formula. */
    seq_t result = 0;
    for (idx_t k = c; k <= r; ++k) {
        seq_t bin1 = binom(c, k - c);   /* C(c, k−c)                 */
        seq_t inner = 0;
        for (idx_t j = 0; j <= k; ++j) {
            /* diff = r - 3*k + 2*j  must be in [0, j]  to contribute. */
            long long t = (long long)r - 3LL * k + 2LL * j;
            if (t < 0)
                continue;
            idx_t diff = (idx_t)t;
            if (diff > j)
                continue;
            seq_t bin2 = binom(j, diff);   /* C(j, diff) */
            seq_t bin3 = binom(k, j);      /* C(k, j)   */
            inner += bin3 * bin2;
        }
        result += bin1 * inner;
    }
    return result;
}
