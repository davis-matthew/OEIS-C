/*
 * A189406  n+[nr/s]+[nt/s]; r=1, s=-1+sqrt(3), t=1+sqrt(3).
 * https://oeis.org/A189406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n(1+√3)/2⌋ + ⌊n(2+√3)⌋
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* For n = 0 (out of range) return 0 just in case. */
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;                     /* convert index to seq_t */
    seq_t s  = floor_sqrt( 3 * nn * nn );    /* s = ⌊n·√3⌋ = ⌊√(3n²)⌋   */
    seq_t w  = nn + s;                       /* w = ⌊n(1+√3)⌋ = n + ⌊n√3⌋ */
    seq_t wh = w >> 1;                       /* ⌊w/2⌋ = ⌊n(1+√3)/2⌋      */

    /* a(n) = 2n + w + ⌊w/2⌋  */
    return 2 * nn + w + wh;
}
