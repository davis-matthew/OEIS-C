/*
 * A211647  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^2+x^2+y^2<=3n.
 * https://oeis.org/A211647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of ordered triples (w,x,y) with 1 ≤ w,x,y ≤ n
 *            and w² + x² + y² ≤ 3n
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root, floor, for unsigned 64‑bit values. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid > n / mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t three_n = n * 3;
    idx_t w_limit = isqrt_u64(three_n);
    if (w_limit > n) w_limit = n;

    for (idx_t w = 1; w <= w_limit; ++w) {
        idx_t ws = w * w;
        idx_t rem1 = three_n - ws;
        idx_t x_limit = isqrt_u64(rem1);
        if (x_limit > n) x_limit = n;

        for (idx_t x = 1; x <= x_limit; ++x) {
            idx_t rem2 = rem1 - x * x;
            if (rem2 < 1) continue;
            idx_t y_limit = isqrt_u64(rem2);
            if (y_limit > n) y_limit = n;
            count += (seq_t) y_limit;
        }
    }
    return count;
}
