/*
 * A036705  Number of Gaussian integers z=a+bi satisfying n - 1/2 < |z| <= n +
 *          1/2.
 * https://oeis.org/A036705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of lattice points (x,y) with n-1/2 < sqrt(x^2+y^2)
 *            <= n+1/2. Equivalent: count of integer pairs with n^2-n+1 <=
 *            x^2+y^2 <= n^2+n
 * Range:     u8 n <= 39, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor square root for 64‑bit unsigned values */
static inline idx_t floor_sqrt_u64(idx_t n) {
    idx_t lo = 0, hi = 0xFFFFFFFFULL, mid, ans = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    /* Special case n == 0 */
    if (n == 0) return 1;

    /* Bounds on the squared norm */
    idx_t r_in  = n * n - n + 1;
    idx_t r_out = n * n + n;

    /* Largest possible |x| : floor(sqrt(r_out)) */
    idx_t xmax = floor_sqrt_u64(r_out);

    unsigned long long result = 0;  /* use 64‑bit accumulator */

    for (idx_t x = 0; x <= xmax; ++x) {
        idx_t x2 = x * x;

        /* Compute bounds for y^2 */
        idx_t lowY  = (x2 >= r_in) ? 0 : r_in - x2;
        idx_t highY = r_out - x2;
        if (highY < lowY) continue;           /* no solutions for this x */

        /* Count integer y with lowY <= y^2 <= highY */
        idx_t sqrt_high = floor_sqrt_u64(highY);
        unsigned long long cnt_y;
        if (lowY == 0) {
            /* Include y = 0 once */
            cnt_y = (unsigned long long)2 * sqrt_high + 1;
        } else {
            idx_t sqrt_low_minus1 = floor_sqrt_u64(lowY - 1);
            cnt_y = (unsigned long long)2 * (sqrt_high - sqrt_low_minus1);
        }

        /* Account for symmetric x values */
        if (x == 0) {
            result += cnt_y;
        } else {
            result += cnt_y * 2;
        }
    }

    return (seq_t)result;
}
