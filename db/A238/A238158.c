/*
 * A238158  Bicycle lock numbers: triangle T(n,k) with 1<=k<=n, where T(n,k)
 *          is the maximum value of min{xy, (n-x)(k-y)} over 0 <= x <= n, 0 <=
 *          y <= k for integers x, y.
 * https://oeis.org/A238158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = triangle entry T(row,col) where T(r,c) = max{min{xy,
 *            (r-x)(c-y)} : 0 <= x <= r, 0 <= y <= c}. The sequence a(n)
 *            enumerates the triangle row by row
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* column index k (1‑based) within the row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    /* compute T(r,k) by scanning over possible x */
    seq_t maxVal = 0;

    /* if either dimension is 1, the maximum is 0 */
    if (r <= 1 || k <= 1) {
        return 0;
    }

    for (idx_t x = 1; x < r; ++x) {
        idx_t rem = r - x;
        idx_t t = rem * k;          /* (n-x)*k */
        idx_t y_low  = t / r;       /* floor((n-x)*k / r) */
        idx_t y_high = (t + r - 1) / r; /* ceil((n-x)*k / r) */

        if (y_low > k) y_low = k;
        if (y_high > k) y_high = k;

        /* evaluate y_low */
        {
            idx_t y = y_low;
            seq_t val1 = (seq_t)x * (seq_t)y;
            seq_t val2 = (seq_t)rem * (seq_t)(k - y);
            seq_t val  = val1 < val2 ? val1 : val2;
            if (val > maxVal) maxVal = val;
        }

        /* evaluate y_high if it differs from y_low */
        if (y_high != y_low) {
            idx_t y = y_high;
            seq_t val1 = (seq_t)x * (seq_t)y;
            seq_t val2 = (seq_t)rem * (seq_t)(k - y);
            seq_t val  = val1 < val2 ? val1 : val2;
            if (val > maxVal) maxVal = val;
        }
    }

    return maxVal;
}
