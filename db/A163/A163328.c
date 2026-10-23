/*
 * A163328  Square array A, where entry A(y,x) has the ternary digits of x
 *          interleaved with the ternary digits of y, converted back to
 *          decimal. Listed by antidiagonals: A(0,0), A(0,1), A(1,0), A(0,2),
 *          A(1,1), A(2,0), ...
 * https://oeis.org/A163328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = interleave(k-t, t) where k = floor((sqrt(8*n+1)-1)/2) and
 *            t = n - k*(k+1)/2. The pair (x, y) = (k-t, t) is obtained by
 *            mapping the index n to antidiagonal coordinates of the table
 *            A(y,x) (0‑based). The interleaving interleaves the ternary
 *            digits of x and y with the least‑significant digit...
 * Range:     u8 n <= 89, u16 n <= 3320, u32 n <= 3320, u64 n <= 3320,
 *            u128 n <= 3320, big any n
 * Helpers:   interleave_xy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/interleave_xy.h"

seq_t a(idx_t n)
{
    /* find the smallest k such that (k+1)(k+2)/2 > n  (i.e., n lies
       in antidiagonal k) */
    idx_t low = 0;
    idx_t high = 1;
    while (((unsigned __int128)(high + 1) * (high + 2)) / 2 <= n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 cum = ((unsigned __int128)(mid + 1) * (mid + 2)) / 2;
        if (cum > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low;

    unsigned __int128 prev_cum = ((unsigned __int128)k * (k + 1)) / 2;
    idx_t t = (idx_t)(n - prev_cum);   /* position within diagonal k */
    idx_t y = t;
    idx_t x = k - t;                   /* remaining position */

    return interleave_xy(x, y);
}
