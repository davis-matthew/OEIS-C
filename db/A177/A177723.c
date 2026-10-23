/*
 * A177723  Number of line segments connecting exactly 7 points in an n X n
 *          grid of points.
 * https://oeis.org/A177723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of points in an n×n grid that
 *            are exactly 7 lattice points apart. Equivalently, the number of
 *            pairs of grid points whose connecting segment contains exactly 7
 *            lattice points, i.e. gcd(|dx|,|dy|) = 6, where (dx,dy) is the
 *            difference vector between the two points
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t ugcd(idx_t a, idx_t b)
{
    /* Euclidean algorithm for unsigned 64‑bit integers. */
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 7) return (seq_t)0;          /* No vector of length 6 fits. */

    idx_t N = n - 1;                     /* Maximum absolute difference. */
    idx_t M = N / 6;                     /* Maximum |u| or |v|, where dx = 6u. */

    seq_t result = 0;

    /* u runs from 0 to M.
       For u = 0 we only allow v > 0 (vertical vectors).
       For u > 0 we allow v in full range -M..M (positive or negative). */
    for (int64_t u = 0; u <= (int64_t)M; ++u)
    {
        /* Compute |u| for gcd and absdx. */
        idx_t abs_u = (idx_t)u;          /* u is non‑negative here. */

        /* Precompute dx and its absolute value. */
        idx_t dx = 6ULL * abs_u;
        if (dx > N) continue;           /* Skip if out of bounds (should not happen). */
        idx_t n_minus_dx = n - dx;       /* n - |dx|, always >= 0 */

        /* Determine range of v */
        int64_t v_min = -(int64_t)M;
        int64_t v_max = (int64_t)M;

        for (int64_t v = v_min; v <= v_max; ++v)
        {
            /* Skip non‑positive v when u == 0 (vertical vectors must go up). */
            if (u == 0 && v <= 0) continue;

            /* Compute |v| for gcd and absdy. */
            idx_t abs_v = (idx_t)(v < 0 ? -v : v);

            /* Skip if points are not coprime (gcd must be 1). */
            if (ugcd(abs_u, abs_v) != 1) continue;

            /* Compute dy and check bounds. */
            idx_t dy = 6ULL * abs_v;
            if (dy > N) continue;         /* Skip out of bounds (should not happen). */
            idx_t n_minus_dy = n - dy;    /* n - |dy| */

            /* Add the number of placements for this direction vector. */
            result += (seq_t)n_minus_dx * (seq_t)n_minus_dy;
        }
    }

    return result;
}
