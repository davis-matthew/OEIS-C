/*
 * A201053  Nearest cube.
 * https://oeis.org/A201053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = nearest cube to n (k^3 if |n-k^3| <= |(k+1)^3‑n|, else
 *            (k+1)^3)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 7945 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    /* floor cube root of n */
    seq_t n_seq = (seq_t)n;
    seq_t k = floor_cube_root(n_seq);          /* k^3 ≤ n < (k+1)^3 */
    seq_t cube_low = k * k * k;                /* k^3 */
    seq_t delta = n_seq - cube_low;            /* distance from n downwards */
    /* difference between successive cubes: (k+1)^3 – k^3 = 3k^2 + 3k + 1 */
    seq_t diff = 3 * k * k + 3 * k + 1;
    seq_t dist_up = diff - delta;              /* distance from n upwards */

    if (delta <= dist_up)
        return cube_low;                       /* choose lower cube */
    else
        return cube_low + diff;                /* choose upper cube ((k+1)^3) */
}
