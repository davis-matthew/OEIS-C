/*
 * A168339  a(n) is the least number of squares needed to form n edge-disjoint
 *          1 X 1 holes inside a rectangle of squares with a solid border.
 * https://oeis.org/A168339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) is the least number of squares in a rectangle that can
 *            contain * n edge‑disjoint 1×1 holes. Equivalently, * a(n) =
 *            min_{x,y>0} (x+2)(y+2) – n * subject to ceil(x*y/2) ≥ n (i.e.
 *            x*y ≥ 2n–1). * This function implements that formula directly
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)          /* integer sqrt: floor(sqrt(n)) */
{
    idx_t lo = 0, hi = n, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety, not used by OEIS offset */
    const idx_t thresh = 2 * n - 1;            /* minimal product xy */
    const idx_t maxX = isqrt_u64(thresh);      /* x up to sqrt(thresh) */

    __uint128_t best_area = (__uint128_t)-1;    /* large unsigned placeholder */

    for (idx_t x = 1; x <= maxX; ++x) {
        const idx_t y = (thresh + x - 1) / x;   /* ceil(thresh / x) */
        const __uint128_t area = (__uint128_t)(x + 2) * (y + 2);
        if (area < best_area) best_area = area;
    }

    /* Convert to seq_t, the required return type */
    return (seq_t)(best_area - ( (__uint128_t)n ));
}
