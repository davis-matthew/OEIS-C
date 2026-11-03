/*
 * A144968  Number of squares between consecutive cubes.
 * https://oeis.org/A144968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((n+1)^(3/2)) - ceil(n^(3/2))
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 410, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9743 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* compute ceil(sqrt(x)) for any positive seq_t */
static inline seq_t ceil_sqrt(seq_t x) {
    seq_t fl = floor_sqrt(x);
    if (fl * fl == x) {
        return fl;
    } else {
        return fl + 1;
    }
}

seq_t a(idx_t n) {
    seq_t nse = (seq_t)n;                 /* current index as seq_t */
    seq_t cube0 = nse * nse * nse;        /* n^3 */
    seq_t c0 = ceil_sqrt(cube0);          /* ceil(n^(3/2)) */

    idx_t m = n + 1;                      /* n+1 */
    seq_t mse = (seq_t)m;
    seq_t cube1 = mse * mse * mse;        /* (n+1)^3 */
    seq_t c1 = ceil_sqrt(cube1);          /* ceil((n+1)^(3/2)) */

    return c1 - c0;
}
