/*
 * A160326  Number of ways to express n=0,1,2,... as the sum of two squares
 *          and a pentagonal number.
 * https://oeis.org/A160326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = x^2 + y^2 + (3z^2 - z)/2 with
 *            x,y,z ≥ 0
 * Range:     u8 n <= 63, u16 n <= 8983, u32 n <= 50000, u64 n <= 50000,
 *            u128 n <= 50000, big any n
 * Validated: standard, pass, 117 terms through n = 8983 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
