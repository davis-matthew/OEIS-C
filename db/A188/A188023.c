/*
 * A188023  Triangle read by rows, T(n,k) = k*A115361(n-1,k-1).
 * https://oeis.org/A188023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   a(n) = k if A115361(m,j)=1 else 0 where m = r-1 (row index of
 *            the A115361 triangle) j = k-1 (column index in that row) r =
 *            smallest integer with r(r+1)/2 >= n (row of a(n)) A115361(m,j)
 *            is defined as: • If m is even → 1 only when j = m (diagonal),
 *            otherwise 0. • If m is odd → let M = m+1. It equa...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not part of the sequence */

    /* find the row r:  r(r+1)/2 >= n */
    idx_t r = 1;
    while (( (__int128)r * (r + 1) ) / 2 < n) {
        ++r;
    }

    /* offset of n within that row (1‑based) */
    idx_t prev_tri = (idx_t)(( (__int128)(r - 1) * r ) / 2);
    idx_t k = n - prev_tri;               /* 1‑based column index */
    idx_t j = k - 1;                      /* 0‑based column index in A115361 */
    idx_t m = r - 1;                      /* row index in A115361 */

    /* decide if A115361(m,j) == 1 */
    if ((m & 1U) == 0) {                  /* m even: only diagonal */
        if (j == m) {
            return (seq_t)k;
        }
    } else {                              /* m odd */
        idx_t M = m + 1;
        idx_t denom = j + 1;
        if (M % denom == 0) {
            unsigned pc_M = __builtin_popcountll((uint64_t)M);
            unsigned pc_d = __builtin_popcountll((uint64_t)denom);
            if (pc_M == pc_d) {
                return (seq_t)k;
            }
        }
    }

    return (seq_t)0;
}
