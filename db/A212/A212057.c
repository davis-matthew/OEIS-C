/*
 * A212057  Number of (w,x,y,z) with all terms in {1,...,n} and w<x*y*z.
 * https://oeis.org/A212057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^4 - ((n+1)D(n) - S(n)) D(n) = Σ_{x=1..n} Σ_{y=1..⌊n/x⌋}
 *            ⌊n/(x*y)⌋ (number of triples with product ≤ n) S(n) = Σ_{x=1..n}
 *            Σ_{y=1..⌊n/x⌋} x*y*⌊n/(x*y)⌋*(⌊n/(x*y)⌋+1)/2 ( sums of the
 *            products of those triples )
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle small n: n=0,1 -> 0 */
    if (n < 2)
        return 0;

    seq_t D = 0;    /* number of triples with product ≤ n */
    seq_t S = 0;    /* sum of products of such triples */

    /* double loop over x and y */
    idx_t x;
    for (x = 1; x <= n; ++x) {
        idx_t maxy = n / x;          /* maximum y for which x*y ≤ n */
        if (maxy == 0)
            continue;

        idx_t y;
        for (y = 1; y <= maxy; ++y) {
            idx_t xy = x * y;                /* x*y ≤ n */
            idx_t m = n / xy;                /* number of z with xy*z ≤ n */

            /* D accumulates m */
            D += (seq_t)m;

            /* S accumulates xy * m*(m+1)/2 */
            seq_t xy_seq = (seq_t)(xy);
            seq_t m_seq = (seq_t)m;
            seq_t sumz = m_seq * (m_seq + 1) / 2;
            S += xy_seq * sumz;
        }
    }

    /* compute n^4 in seq_t form */
    seq_t n_seq = (seq_t)n;
    seq_t n4 = n_seq * n_seq * n_seq * n_seq;

    /* final formula a(n) = n^4 - ((n+1)D - S) */
    seq_t res = n4 - ((seq_t)(n + 1) * D - S);
    return res;
}
