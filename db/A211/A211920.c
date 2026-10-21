/*
 * A211920  Number of (w,x,y,z) with all terms in {1,...,n} and 2*w*x<3*y*z.
 * https://oeis.org/A211920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (w,x,y,z) : 1≤w,x,y,z≤n and 2*w*x < 3*y*z }| The
 *            function computes the number of ordered quadruples that satisfy
 *            the inequality by counting product frequencies of ordered pairs
 *            (w,x) and (y,z). Since both ranges are the same, the same
 *            frequency table is used for both sides: Cp = Dp = nu...
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* offset 0 */
        return 0;

    /* maximum product that can appear */
    idx_t maxProd = n * n;

    /* Allocate array of product frequencies.
       Use uint16_t as all counts fit into 16 bits for n ≤ 10000. */
    size_t sz = (size_t)maxProd + 1u;
    uint16_t *c = (uint16_t *)calloc(sz, sizeof(uint16_t));
    if (!c)
        return 0; /* allocation failed */

    /* Count ordered pairs (w,x) with product p */
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            ++c[w * x];
        }
    }

    seq_t totalPairs = (seq_t)n * (seq_t)n;
    seq_t ans = 0;
    seq_t cumD = 0;          /* cumulative sum of Dq for q ≤ current t   */
    idx_t next = 1;          /* next index to include in cumD           */

    /* Scan p from 1 to maxProd in ascending order.
       For each p, threshold t = floor(2p/3); we maintain cumD as the
       sum of c[q] for q ≤ t.  Then suffix sum of q > t is
       totalPairs - cumD.                                            */
    for (idx_t p = 1; p <= maxProd; ++p) {
        idx_t t = (2 * p) / 3;
        while (next <= t) {
            cumD += (seq_t)c[next];
            ++next;
        }
        seq_t suffix = totalPairs - cumD;
        ans += (seq_t)c[p] * suffix;
    }

    free(c);
    return ans;
}
