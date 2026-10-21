/*
 * A083088  First column of table A083087.
 * https://oeis.org/A083088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / sqrt(2)) + n + 1, n >= 0
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   idx_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/idx_sqrt.h"

seq_t a(idx_t n)
{
    /* work in seq_t for the final result; use idx_t for intermediates
       so that large n do not overflow seq_t even for 8‑bit builds      */
    idx_t nn   = n;
    idx_t sq   = nn * nn;          /* n^2  (fits in idx_t for n <= 10^4)  */
    idx_t half = sq / 2;           /* floor(n^2 / 2)  */
    seq_t t    = idx_sqrt(half);   /* floor(n / sqrt(2))   */
    return (seq_t)(nn + t + 1);
}
