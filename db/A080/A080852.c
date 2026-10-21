/*
 * A080852  Square array of 4D pyramidal numbers, read by antidiagonals.
 * https://oeis.org/A080852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) * (n,k ≥ 0)
 *            along antidiagonals. * T(n,k) = binom(k+4,4) +
 *            (n-1)*binom(k+3,4) * The mapping of index n to the pair (r,c) in
 *            the array is: * d = the diagonal index such that (d+1)(d+2)/2 >
 *            n * t = n - d(d+1)/2 (0 ≤ t ≤ d) * r = d - t (row index)...
 * Range:     u8 n <= 40, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom4.h"

seq_t a(idx_t n)
{
    /* find diagonal d such that (d+1)(d+2)/2 > n */
    uint64_t d = 0;
    while ( (__int128)(d+1) * (d+2) / 2 <= n )
        ++d;
    /* cumulative count before this diagonal */
    uint64_t cum_prev = (uint64_t)((__int128)d * (d+1) / 2);
    uint64_t t = n - cum_prev;
    uint64_t r = d - t;   /* row index */
    uint64_t c = t;       /* column index */

    seq_t base = binom4(c + 4);        /* C(k+4,4)   */
    seq_t inc  = binom4(c + 3);        /* C(k+3,4)   */

    if (r == 0)
        return base - inc;               /* T(0,k)   */
    else
        return base + (seq_t)(r - 1) * inc;  /* T(r,k), r≥1 */
}
