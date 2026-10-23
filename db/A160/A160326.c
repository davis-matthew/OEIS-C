/*
 * A160326  Number of ways to express n=0,1,2,... as the sum of two squares
 *          and a pentagonal number.
 * https://oeis.org/A160326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = x^2 + y^2 + (3z^2 - z)/2 with
 *            x,y,z ≥ 0
 * Range:     u8 n <= 63, u16 n <= 8983, u32 n <= 8983, u64 n <= 8983,
 *            u128 n <= 8983, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t total = 0;
    seq_t n_seq = (seq_t)n;

    /* Iterate over z until the pentagonal number exceeds n. */
    for (idx_t z = 0; ; ++z) {
        seq_t z_seq = (seq_t)z;
        /* Compute pentagonal number p(z) = (3z^2 - z)/2 = z*(3z-1)/2 */
        seq_t p = ((seq_t)3 * z_seq * z_seq - z_seq) / (seq_t)2;
        if (p > n_seq) break;

        seq_t m = n_seq - p;          /* Remaining value to be expressed as x^2 + y^2 */
        idx_t xmax = (idx_t)floor_sqrt(m);  /* Maximum possible x value */

        for (idx_t x = 0; x <= xmax; ++x) {
            seq_t xsq = (seq_t)x * (seq_t)x;
            seq_t y2 = m - xsq;
            seq_t y  = floor_sqrt(y2);
            if (y * y == y2) {
                total++;
            }
        }
    }

    return total;
}
