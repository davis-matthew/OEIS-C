/*
 * A224837  Surface area of Johnson square pyramid (rounded down) with all the
 *          edge-lengths equal to n.
 * https://oeis.org/A224837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1+sqrt(3))*n^2)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 197, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    /* n² = n * n, we cast n to seq_t first */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    /* X = 3 * n⁴ = 3 * n_sq * n_sq */
    seq_t X = 3 * n_sq * n_sq;
    /* floor(sqrt(3) * n²) = floor(sqrt(3 * n⁴)) = floor_sqrt(X) */
    seq_t sqrt_floor = floor_sqrt(X);
    /* Final result: n² + floor(sqrt(3) * n²) */
    return n_sq + sqrt_floor;
}
