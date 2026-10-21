/*
 * A216673  Total number of solutions to the equation x^2 + k*y^2 = n with x
 *          >= 0, y >= 0, k > 0, or 0 if the number is infinite. (Order does
 *          not matter for the equation x^2 + y^2 = n).
 * https://oeis.org/A216673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a perfect square otherwise: a(n) =
 *            Σ_{x=0}^{⌊√n⌋} Σ_{y=1, y² | (n−x²)}¹ where the inner sum counts
 *            1 for each y such that y² divides (n−x²); if the implied k =
 *            (n−x²)/y² equals 1, the pair (x, y) is counted only once (i.e. x
 *            ≤ y) to avoid duplicating the symmetry of x² + y² = n
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* integer square root for 64‑bit unsigned integers */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    /* sqrt(2^64−1) < 2^32  →  upper bound 2^32 */
    idx_t high = ((idx_t)1 << 32);
    if (high > n)
        high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned long long sq = (unsigned long long)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Check the infinite‑solution case (n a perfect square). */
    idx_t r = isqrt_u64(n);
    if ((unsigned long long)r * r == n)
        return 0;

    seq_t total = 0;
    for (idx_t x = 0; x <= r; ++x) {
        idx_t m = n - x * x;          /* m > 0 since n is not a square */
        if (m == 0)
            continue;                 /* safety, should not occur */
        idx_t y_max = isqrt_u64(m);
        for (idx_t y = 1; y <= y_max; ++y) {
            idx_t ysq = y * y;
            if (m % ysq == 0) {
                idx_t k = m / ysq;
                if (k == 1) {
                    /* unordered pair for k = 1 */
                    if (x <= y)
                        total += 1;
                } else {
                    total += 1;         /* ordered pair for k > 1 */
                }
            }
        }
    }
    return total;
}
