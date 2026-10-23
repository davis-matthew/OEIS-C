/*
 * A103582  Binary array below read by downward antidiagonals.
 * https://oeis.org/A103582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 - (((c >> r) & 1)) where r = n - d*(d+1)/2, d =
 *            floor((sqrt(8*n+1)-1)/2), c = d - r
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   tri_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A103582. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/tri_leq.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (tri_leq(mid, n))
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t d = lo;

    /* Compute t = d*(d+1)/2 (triangular number of d) */
    unsigned __int128 t128 = (unsigned __int128)d * (d + 1) / 2;
    idx_t t = (idx_t)t128;

    /* Row = offset within the diagonal, column = remaining */
    idx_t row = n - t;
    idx_t col = d - row;

    /* For row >= 64, shifting would be undefined, but
       2^row > max col for any valid col, so block index is 0. */
    if (row >= 64)
        return (seq_t)1;

    idx_t block = col >> row;
    return (seq_t)(1 - (block & 1));
}
