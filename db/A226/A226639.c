/*
 * A226639  a(n) = n^4/8 + (5*n^3)/12 - n^2/8 - (5*n)/12 + 1.
 * https://oeis.org/A226639
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4/8 + (5*n^3)/12 - n^2/8 - (5*n)/12 + 1
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses integer arithmetic with a common denominator of 24:
   a(n) = (3*n^4 + 10*n^3 - 3*n^2 - 10*n + 24) / 24
   The numerator is always divisible by 24 for integer n. */

#include <stdint.h>

/* Compute a(n) for n >= -1. */
seq_t a(idx_t n)
{
    /* Convert index to signed 128‑bit for safe arithmetic. */
    __int128 nn = (int64_t)n;          /* idx_t is signed 64‑bit */
    __int128 n2 = nn * nn;             /* n^2 */
    __int128 n3 = n2 * nn;             /* n^3 */
    __int128 n4 = n3 * nn;             /* n^4 */

    __int128 num = 3 * n4 + 10 * n3 - 3 * n2 - 10 * nn + 24;
    /* num is guaranteed to be non‑negative for n >= -1.   */
    return (seq_t)(num / 24);
}
