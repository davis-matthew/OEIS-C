/*
 * A033781  Product t2(q^d); d | 22, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of solutions in nonnegative integers (a,b,c,d) to
 *            a*(a+1) + 2*b*(b+1) + 11*c*(c+1) + 22*d*(d+1) = 2*n. This
 *            follows from the product Π_{d|22} t2(q^d), t2(q) =
 *            θ2(q)/(2*q^(1/4)) where θ2(q) = 2 Σ_{k≥0} q^{(k+1/2)^2} and
 *            hence t2(q^d) = Σ_{k≥0} q^{d*k*(k+1)/2}
 * Range:     u8 n <= 902, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* since all computations stay within 64‑bit range for n ≤ ~1e5,
       we can safely use idx_t for intermediate values.            */
    idx_t M = (idx_t)2 * n;   /* left‑hand side of the equation         */
    seq_t count = 0;          /* number of tuples found                 */

    /* d corresponds to the factor with multiplier 22 */
    for (idx_t d = 0;; ++d) {
        idx_t term_d = 22 * d * (d + 1);
        if (term_d > M) break;
        idx_t rest1 = M - term_d;

        /* c corresponds to multiplier 11 */
        for (idx_t c = 0;; ++c) {
            idx_t term_c = 11 * c * (c + 1);
            if (term_c > rest1) break;
            idx_t rest2 = rest1 - term_c;

            /* b corresponds to multiplier 2 */
            for (idx_t b = 0;; ++b) {
                idx_t term_b = 2 * b * (b + 1);
                if (term_b > rest2) break;
                idx_t rest3 = rest2 - term_b;   /* must equal a*(a+1) */

                /* solve a*(a+1) == rest3  (polynomial in a) */
                /* a is at most sqrt(rest3) which is < ~√(M) */
                for (idx_t a = 0; a * (a + 1) <= rest3; ++a) {
                    if (a * (a + 1) == rest3) {
                        ++count;
                    }
                }
            }
        }
    }

    return count;
}
