/*
 * A232825  Number of (n+1)X(1+1) 0..1 arrays x(i,j) with row sums
 *          sum{j*x(i,j), j=1..1+1} nondecreasing, and column sums
 *          sum{i*x(i,j), i=1..n+1} nondecreasing.
 * https://oeis.org/A232825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer quadruples * (a0,a1,a2,a3)
 *            with a0+a1+a2+a3 = n+1 such that * a1*(2*a0 + a1 + 1) ≤ a2*(2*a0
 *            + 2*a1 + a2 + 1) * * (The counts correspond to the weighted
 *            row/column sum constraints * of the 0/1 array described in the
 *            OEIS entry.)
 * Range:     u8 n <= 10, u16 n <= 82, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    /* m = number of rows = n+1 */
    uint64_t m = (uint64_t)n + 1;
    seq_t total = 0;

    for (uint64_t a0 = 0; a0 <= m; ++a0) {
        uint64_t max_a1 = m - a0;
        for (uint64_t a1 = 0; a1 <= max_a1; ++a1) {
            /* d = 2*a0 + 2*a1 + 1  (appears in both sides of the inequality) */
            uint64_t d   = 2*a0 + 2*a1 + 1;
            /* L = a1*(2*a0 + a1 + 1)  (left side of the inequality) */
            uint64_t L   = a1 * (2*a0 + a1 + 1);

            /* Solve a2*(a2 + d) ≥ L for minimal integer a2 ≥ 0.
             * The quadratic root is t0 = (sqrt(d² + 4L) – d)/2.
             */
            long double disc = (long double)d * (long double)d + 4.0L * (long double)L;
            long double t0   = (sqrtl(disc) - (long double)d) / 2.0L;
            uint64_t a2_min  = (uint64_t)ceill(t0);   /* ceil of t0 */

            uint64_t max_a2 = m - a0 - a1;            /* a3 must remain non‑negative */
            if (a2_min > max_a2) continue;           /* no admissible a2 */

            total += (seq_t)(max_a2 - a2_min + 1);    /* all a2 in [a2_min … max_a2] */
        }
    }

    return total;
}
