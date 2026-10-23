/*
 * A014198  Number of integer solutions to x^2 + y^2 <= n excluding (0,0).
 * https://oeis.org/A014198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer solutions to x^2 + y^2 <= n excluding
 *            (0,0)
 * Range:     u8 n <= 81, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of an unsigned 64‑bit integer. */
static idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else if ((mid + 1) > 0 && (mid + 1) > n / (mid + 1)) {
            return mid;
        } else {
            low = mid + 1;
        }
    }
    return n;   /* pure safety, unreachable */
}

/* OEIS a(n). */
seq_t a(idx_t n)
{
    idx_t R = floor_isqrt_u64(n);   /* maximum |x| or |y| */
    seq_t ans = 0;

    for (idx_t x = 0; x <= R; ++x) {
        idx_t rem  = n - x * x;                /* remaining radius for y */
        idx_t maxy = floor_isqrt_u64(rem);      /* floor(sqrt(rem)) */

        if (x == 0) {
            ans += (seq_t)(2 * maxy);          /* only y ≠ 0 contribute */
        } else {
            ans += (seq_t)(2 + 4 * maxy);       /* y = 0 gives 2 points,
                                                   y > 0 gives 4 each */
        }
    }
    return ans;
}
