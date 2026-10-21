/*
 * A219744  The number of 2 X 2 symmetric positive definite matrices whose
 *          entries are integers x,y,z satisfying x^2 + y^2 + z^2 <= n^2.
 * https://oeis.org/A219744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 2×2 symmetric positive definite matrices * with
 *            integer entries x, y, z such that * x > 0, z > 0, x·z > y² * x²
 *            + y² + z² ≤ n²
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

/* External linkage required by the test harness */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for indices < 1 */

    const seq_t N   = (seq_t)n;
    const seq_t N2  = N * N;
    seq_t result = 0;

    /* iterate over |y|; y = 0 is handled once, other y are doubled */
    for (idx_t y = 0; y <= n; ++y) {
        const seq_t y2 = (seq_t)y * y;
        const seq_t circle_radius_sq = N2 - y2;   /* always >= 0 */

        /* count positive integer (x,z) pairs inside 1st quadrant circle */
        seq_t pair_count = 0;
        const seq_t limit_x = floor_sqrt(circle_radius_sq);

        for (idx_t x = 1; x <= (idx_t)limit_x; ++x) {
            const seq_t x2 = (seq_t)x * x;
            const seq_t remaining_sq = circle_radius_sq - x2;
            const seq_t max_z = floor_sqrt(remaining_sq);

            /* z must satisfy x·z > y²  ⇔  z > y² / x */
            const seq_t thresh = y2 / (seq_t)x;          /* floor division */
            idx_t z_start = (thresh < max_z ? (idx_t)(thresh + 1) : (idx_t)max_z + 1);

            if (z_start <= (idx_t)max_z) {
                pair_count += (seq_t)((idx_t)max_z - z_start + 1);
            }
        }

        /* Add contributions for current |y| */
        if (y == 0)
            result += pair_count;          /* only one sign for y = 0 */
        else
            result += 2 * pair_count;       /* y and –y give the same count */
    }

    return result;
}
