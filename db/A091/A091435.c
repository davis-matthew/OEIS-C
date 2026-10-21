/*
 * A091435  Array T(n,k) = n*(n+k), read by antidiagonals.
 * https://oeis.org/A091435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   find_diagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A091435
    Table T(n,k) = n*(n+k) (n,k ≥ 0) read by antidiagonals (row-first).
    The index n corresponds to a pair (r,c) on diagonal d = r+c:
        d = floor((sqrt(8*n+1)-1)/2)
        p = n - d*(d+1)/2          // position on this diagonal
        r = d - p
        c = p
    Then a(n) = T(r,c) = r*(r+c) = r*d.
*/
#include <math.h>   /* sqrtl */
#include "../utils/find_diagonal.h"

seq_t a(idx_t n)
{
    /* compute diagonal and offset */
    idx_t d = find_diagonal(n);
    unsigned __int128 prev = (unsigned __int128)d * (d + 1) / 2;   /* terms before this diagonal */
    idx_t p = (idx_t)(n - prev);                                 /* position on the diagonal (0…d) */

    /* row (r) and column (c) */
    idx_t r = d - p;
    /* c = p; (not needed for computation) */

    /* final value a(n) = r * d */
    return (seq_t)r * (seq_t)d;
}
