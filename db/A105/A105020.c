/*
 * A105020  Array read by upward antidiagonals: row n (n >= 0) contains the
 *          numbers m^2 - n^2, m >= n+1.
 * https://oeis.org/A105020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = r^2 - (r^2 + r - m)^2/4 * where m = 2*n + 2 and r =
 *            round(sqrt(m))
 * Range:     u8 n <= 35, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Validated: standard, pass, 82 terms through n = 1224 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* m = 2*n + 2 */
    idx_t m_idx = 2 * n + 2;
    seq_t m = (seq_t)m_idx;

    /* r_floor = floor(sqrt(m)) */
    idx_t r_floor = isqrt(m_idx);
    seq_t r = (seq_t)r_floor;
    seq_t r_sq = r * r;

    /* Compute differences to decide round */
    seq_t diff1 = m - r_sq;                      /* m - r^2 */
    seq_t diff2 = (r + 1) * (r + 1) - m;        /* (r+1)^2 - m */
    if (diff1 > diff2) {
        /* round up */
        r = r + 1;
        r_sq = r * r;      /* update r_sq after rounding */
    }

    /* t = r^2 + r - m */
    seq_t t = r_sq + r - m;
    seq_t t_sq = t * t;

    /* return r^2 - t_sq / 4 */
    return r_sq - t_sq / 4;
}
