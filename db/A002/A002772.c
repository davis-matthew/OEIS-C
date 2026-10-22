/*
 * A002772  Number of terms in a bordered skew determinant.
 * https://oeis.org/A002772
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{k=0..n-1} A002771(k)/k!, i.e. a(n) =
 *            (n-1)*a(n-1) + b(n-1), a(1) = 1, where b = A002771 has e.g.f.
 *            (exp(x)/sqrt(1-x^2) + exp(x+x^2/2))/2, computed as b(k) = t(k) +
 *            d(k) with t(k) = t(k-1) + (k-1)*t(k-2), t(0) = t(1) = 1
 *            (involutions), and d(k+1) = d(k) + k^2*d(k-1) - k*(k-1)*...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t A = 1;                                /* a(1) */
    seq_t t0 = 1, t1 = 0, t2 = 0, t3 = 0;       /* t(k), t(k-1), t(k-2), t(k-3); k = 1 */
    seq_t d0 = 0, d1 = 0, d2 = 0;               /* d(k), d(k-1), d(k-2) */
    idx_t m;
    t1 = 1;                                     /* t(0) = 1 */
    if (n < 2) return 1;
    for (m = 2; m <= n; m++) {
        idx_t k = m - 1;                        /* state holds index k */
        uint64_t c1, c2, c3;
        seq_t nt, nd;
        A = (seq_t)(m - 1) * A + (t0 + d0);    /* b(k) = t(k) + d(k) */
        if (m == n) break;
        c1 = (uint64_t)k * (uint64_t)k;
        c2 = (uint64_t)k * (uint64_t)(k - 1);
        c3 = (k >= 2) ? (c2 / 2) * (uint64_t)(k - 2) : 0;
        nt = t0 + (seq_t)k * t1;
        nd = d0 + (seq_t)c1 * d1 + (seq_t)c3 * t3;
        nd -= (seq_t)c2 * d2;
        t3 = t2; t2 = t1; t1 = t0; t0 = nt;
        d2 = d1; d1 = d0; d0 = nd;
    }
    return A;
}
