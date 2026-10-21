/*
 * A213083  Each square n^2 appears n^2 number of times.
 * https://oeis.org/A213083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k^2 where k is the smallest integer with k(k+1)(2k+1)/6
 *            >= n. Equivalently, let m = floor((3n)^(1/3)). If n >
 *            m(m+1)(2m+1)/6 then k = m+1, else k = m
 * Range:     u8 n <= 1240, u16 n <= 9329, u32 n <= 9329, u64 n <= 9329,
 *            u128 n <= 9329, big any n
 * Helpers:   floor_cube_root_3n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_cube_root_3n.h"

seq_t a(idx_t n)
{
    /* n >= 1; offset 1. */
    idx_t m = floor_cube_root_3n(n);
    seq_t mm = (seq_t)m;
    seq_t S = mm * (mm + 1) * (2 * mm + 1) / 6;   /* S = m(m+1)(2m+1)/6 */
    if ((seq_t)n > S) {
        ++m;
    }
    seq_t k = (seq_t)m;
    return k * k;   /* k^2 */
}
