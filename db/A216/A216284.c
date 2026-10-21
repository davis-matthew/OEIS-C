/*
 * A216284  Number of solutions to the equation x^4+y^4 = n with x >= y > 0.
 * https://oeis.org/A216284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to x^4 + y^4 = n with x >= y > 0
 * Range:     u8 n <= 9677, u16 n <= 9677, u32 n <= 9677, u64 n <= 9677,
 *            u128 n <= 9677, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute floor fourth root of n (largest integer r with r^4 <= n). */
static inline idx_t floor_fourth_root(idx_t n)
{
    idx_t low = 0;
    idx_t high = 65535;           /* (2^16-1)^4 < 2^64 */
    idx_t res = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        /* mid^4 */
        uint64_t m = (uint64_t)mid;
        uint64_t m2 = m * m;       /* mid^2 */
        uint64_t m4 = m2 * m2;     /* mid^4 */
        if (m4 <= (uint64_t)n) {
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
    if (n < 2) return 0;          /* n=1 has no positive solutions */

    seq_t count = 0;
    idx_t y_max = floor_fourth_root(n);

    for (idx_t y = 1; y <= y_max; ++y) {
        /* y^4 */
        uint64_t yv = (uint64_t)y;
        uint64_t y2 = yv * yv;
        uint64_t y4 = y2 * y2;

        if (y4 > (uint64_t)n) break;   /* safety, though not needed */

        uint64_t r = (uint64_t)n - y4;  /* remaining part */

        idx_t x = floor_fourth_root(r);

        /* check if x^4 == r */
        uint64_t xv = (uint64_t)x;
        uint64_t x2 = xv * xv;
        uint64_t x4 = x2 * x2;

        if (x4 == r && (idx_t)x >= y) {
            ++count;
        }
    }
    return count;
}
