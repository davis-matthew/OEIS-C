/*
 * A216674  Total number of solutions to the equation x^2+k*y^2 = n with x >
 *          0, y > 0, k >= 0, or 0 if infinite. (Order does not matter for the
 *          equation x^2+y^2 = n).
 * https://oeis.org/A216674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integer triples (x,y,k) satisfying *
 *            x^2 + k*y^2 = n, with x>0, y>0, k>=0; unordered pairs for k=1
 *            counted once. * infinite solutions (k=0) return 0 when n is a
 *            perfect square
 * Range:     u8 n <= 8424, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(n)) for unsigned 64‑bit n */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t res = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0 || mid <= n / mid) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n >= 1 – the sequence starts at offset 1.  For any lower index
     * behaviour is unspecified. */
    if (n == 0) return 0;

    /* Infinite solutions occur only when n is a perfect square
     * (k = 0: x^2 = n, any positive y leads to a solution). */
    if (isqrt_u64(n) * isqrt_u64(n) == n) return 0;

    seq_t total = 0;

    /* For each possible x, compute the remaining value rem = n - x^2.
     * rem must be positive and equal to k*y^2 for some y>0. */
    idx_t max_x = isqrt_u64(n - 1);          /* because x^2 < n */
    for (idx_t x = 1; x <= max_x; ++x) {
        idx_t xsq = x * x;
        idx_t rem = n - xsq;                 /* > 0 */

        idx_t max_y = isqrt_u64(rem);        /* because y^2 <= rem */
        for (idx_t y = 1; y <= max_y; ++y) {
            idx_t ysq = y * y;
            if (rem % ysq != 0) continue;   /* k would not be integer */
            idx_t k = rem / ysq;            /* k >= 1 for rem > 0 */

            if (k == 1) {
                /* unordered pair: count only when x >= y */
                if (x >= y) ++total;
            } else {
                /* ordered pair for k > 1 */
                ++total;
            }
        }
    }
    return total;
}
