/*
 * A211428  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and 2*w^2+3*x*y=0.
 * https://oeis.org/A211428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = number of ordered triples (w,x,y) with w,x,y ∈ [-n..n]
 *            satisfying * 2*w^2 + 3*x*y = 0. * This is computed directly by
 *            examining w multiples of 3 and counting * factor pairs for x*y =
 *            -(2/3)w^2 = -6*(w/3)^2
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base case: w = 0 gives xy = 0 */
    seq_t result = (seq_t)4 * n + 1;

    /* w must be a multiple of 3: w = ±3k,   1 ≤ k ≤ floor(n/3) */
    idx_t max_k = n / 3;
    for (idx_t k = 1; k <= max_k; ++k) {
        /* product that must be obtained by x*y */
        seq_t m = (seq_t)6 * ((seq_t)k * (seq_t)k);

        /* count ordered positive factor pairs (d,e) with d*e = m
         * such that d ≤ n and e ≤ n.  Each such pair gives
         * two sign assignments for (x,y), and both signs of w
         * contribute, so we add 4 per pair. */
        seq_t cnt = 0;
        for (idx_t d = 1; d <= n; ++d) {
            if (m % d == 0) {
                seq_t e = m / d;
                if (e <= n) {
                    ++cnt;
                }
            }
        }
        result += cnt * 4;
    }
    return result;
}
