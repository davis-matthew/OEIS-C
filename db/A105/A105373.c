/*
 * A105373  Square array by antidiagonals of number of straight lines with n
 *          points in a k-dimensional hypercube with n points on each edge.
 * https://oeis.org/A105373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where * T(r,c) = ((r+2)^c - r^c)/2 * and row,
 *            col are obtained by listing the array T(r,c) by antidiagonals: *
 *            (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),... * This maps the n‑th
 *            term of OEIS A105373
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   T_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/T_val.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* offset is 1 */
    /* Find the minimal s with s*(s-1)/2 >= n */
    uint64_t s = (uint64_t)((1 + sqrt((double)1 + 8.0 * (double)n)) / 2);
    while (((__int128)s * (s - 1)) / 2 < (__int128)n)
        ++s;
    /* previous total up to sum (s-1) */
    __int128 prev = ((__int128)(s - 1) * (s - 2)) / 2;
    uint64_t pos = (uint64_t)(((__int128)n - prev) - 1);
    uint64_t r   = pos + 1;
    uint64_t c   = s - r;
    return T_val(r, c);
}
