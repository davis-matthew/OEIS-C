/*
 * A028961  Theta series of quadratic form (or lattice) with Gram matrix [ 3,
 *          1; 1, 8 ].
 * https://oeis.org/A028961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x, y) to 3*x^2 + 2*x*y +
 *            8*y^2 = n
 * Range:     u8 n <= 2215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for 0 ≤ x ≤ 2^63‑1 (returns floor(√x)).  Works for the
   small numbers needed (D < 4·10^5 for n ≤ 10000). */
static seq_t isqrt_uint64(uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return (seq_t)r;          /* r ≤ 565 for the relevant n, fits in any seq_t */
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* From 32*n – 92*x^2 ≥ 0 we get x^2 ≤ (32/92)*n = (8/23)*n.
       For n ≤ 10000 this bound is at most 25.39, so |x| ≤ 5 for the
       largest n in the benchmark.  Use that tighter bound. */
    uint64_t max_x_sq = (n * 32ULL) / 92ULL;   /* floor((8*n)/23) */
    int max_x = (int)isqrt_uint64(max_x_sq);

    for (int x = -max_x; x <= max_x; ++x)
    {
        uint64_t xx = (x >= 0) ? (uint64_t)x : (uint64_t)(-x);
        uint64_t D  = 32ULL * n - 92ULL * xx * xx;   /* Δ = 32*n – 92*x^2 */

        if (D > 32ULL * n)   /* should never happen, safety check */
            continue;

        seq_t s = isqrt_uint64(D);
        if ((uint64_t)s * (uint64_t)s != D)
            continue;              /* D is not a perfect square */

        /* solve 8*y^2 + 2*x*y + (3*x^2 – n) = 0  →  y = (-2x ± s)/16 */
        int64_t num1 = -2LL * x + (int64_t)s;
        if (num1 % 16 == 0)
            ++count;              /* first solution */

        if (s != 0)
        {
            int64_t num2 = -2LL * x - (int64_t)s;
            if (num2 % 16 == 0)
                ++count;          /* second, distinct solution */
        }
    }

    return count;
}
