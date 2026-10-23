/*
 * A119438  Number of sets of points determined by the intersection of a line
 *          with an n X n grid of points.
 * https://oeis.org/A119438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A018808(n) + n^2 + 1 A018808(n) = number of lines in an
 *            n×n integer grid that contain at least two grid points
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* offset 0 */
    if (n == 0)
        return (seq_t)1;

    seq_t lines_sum = 0;

    /* vertical and horizontal lines (only when n ≥ 2) */
    if (n >= 2) {
        lines_sum += (seq_t)n; /* vertical lines */
        lines_sum += (seq_t)n; /* horizontal lines */
    }

    /* primitive directions with dx ≥ 1, dy ≥ 1 */
    for (idx_t dx = 1; dx < n; ++dx) {
        seq_t x_term = (seq_t)(n - dx);
        seq_t t1 = (seq_t)((n > 2 * dx) ? (n - 2 * dx) : 0);

        for (idx_t dy = 1; dy < n; ++dy) {
            if (gcd_idx(dx, dy) != 1)
                continue;                     /* not primitive */

            seq_t y_term = (seq_t)(n - dy);
            seq_t t2 = (seq_t)((n > 2 * dy) ? (n - 2 * dy) : 0);
            seq_t lines = x_term * y_term - t1 * t2;
            if (lines > 0) {
                lines_sum += lines * (seq_t)2; /* both +dy and –dy slopes */
            }
        }
    }

    /* final value: A018808(n) + n^2 + 1 */
    return lines_sum + (seq_t)n * (seq_t)n + (seq_t)1;
}
