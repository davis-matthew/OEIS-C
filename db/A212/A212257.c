/*
 * A212257  Number of (v,w,x,y,z) with all terms in {0,1,...,n} and
 *          v=average(w,x,y,z).
 * https://oeis.org/A212257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{v=0}^{n} f(4v) * where f(S) = # { (w,x,y,z) |
 *            0≤w,x,y,z≤n , w+x+y+z = S } * f(S) = Σ_{j=0}^{4} (-1)^j * C(4,j)
 *            * C(S - j*(n+1) + 3, 3) * (empty binomials are treated as 0)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned int C4[5] = {1, 4, 6, 4, 1};
    seq_t total = 0;

    for (idx_t v = 0; v <= n; ++v) {
        idx_t S = v * 4;
        seq_t pos = 0, neg = 0;

        for (int j = 0; j <= 4; ++j) {
            int64_t m = (int64_t)S - (int64_t)j * ((int64_t)n + 1) + 3;
            if (m < 3) continue; /* binomial is 0 */

            seq_t bm = (seq_t)m;
            seq_t t   = bm * (bm - 1) * (bm - 2);   /* m*(m-1)*(m-2) */
            t /= 6;                                 /* divide by 3! */

            seq_t term = t * C4[j];

            if (j & 1)
                neg += term;
            else
                pos += term;
        }

        total += pos - neg;   /* pos >= neg */
    }

    return total;
}
