/*
 * A216282  Number of nonnegative solutions to the equation x^2 + 2*y^2 = n.
 * https://oeis.org/A216282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9942, u16 n <= 9942, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of nonnegative integer solutions (x, y) to x^2 + 2*y^2 = n */
#include <stdint.h>
#include <math.h>

/* Integer square‑root of a 64‑bit unsigned integer. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = (idx_t)sqrt((long double)n);
    while (( (__uint128_t)(x + 1) * (x + 1) ) <= n) ++x;
    while (( (__uint128_t)x * x ) > n) --x;
    return x;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) /* trivial case: 0 = 0^2 + 2*0^2 */
        return 1;
    idx_t max_x = isqrt_u64(n);
    for (idx_t x = 0; x <= max_x; ++x)
    {
        idx_t xsq = x * x;
        idx_t rem = n - xsq;
        if ((rem & 1ULL) != 0)          /* r must be even */
            continue;
        idx_t y2 = rem >> 1;            /* y^2 = (n - x^2)/2 */
        idx_t y  = isqrt_u64(y2);
        if (y * y == y2)                /* y^2 exactly equals y2 */
            ++count;
    }
    return count;
}
