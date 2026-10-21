/*
 * A213668  Irregular triangle read by rows: T(n,k) is the number of
 *          dominating subsets with k vertices of the graph G(n) consisting of
 *          a pair of endvertices joined by n internally disjoint paths of
 *          length 2 (the n-ary generalized theta graph THETA_{2,2,...2};
 *          n>=1, 1<=k<=n+2).
 * https://oeis.org/A213668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = coefficient of x^k in ((1+x)^r-1)*((1+x)^2-1)+x^r+x^2,
 *            where r = smallest integer with r(r+5)/2 ≥ n and k = n -
 *            (r-1)(r+4)/2
 * Range:     u8 n <= 56, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Find the row r such that (r-1)(r+4)/2 < n ≤ r(r+5)/2. */
    double d = sqrt((double)8 * n + 25.0);
    idx_t r = (idx_t)((d - 5.0) / 2.0);
    if (r < 1) r = 1;
    while (r * (r + 5) / 2 < n) ++r;
    while (r > 1 && ((r - 1) * (r + 4) / 2) >= n) --r;

    idx_t prev = (r - 1) * (r + 4) / 2; /* cumulative terms before row r */
    idx_t k = n - prev;                 /* position within row r (1‑based) */

    seq_t coeff = 0;

    if (k >= 2) {
        seq_t t = binom(r, k - 1);
        coeff += t;
        coeff += t;                     /* 2·binom(r,k−1) */
    }
    if (k >= 3) {
        coeff += binom(r, k - 2);       /* +binom(r,k−2) */
    }
    if (k == r) coeff += 1;             /* +x^r term */
    if (k == 2) coeff += 1;             /* +x^2 term */

    return coeff;
}
