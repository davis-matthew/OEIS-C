/*
 * A232423  a(n) = ceiling(sqrt(n^4 - n^3 - n^2 + n + 1))^2 - (n^4 - n^3 - n^2
 *          + n + 1).
 * https://oeis.org/A232423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(n^4 - n^3 - n^2 + n + 1))^2 - (n^4 - n^3 - n^2
 *            + n + 1)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2  = nn * nn;          /* n^2 */
    seq_t n3  = n2 * nn;          /* n^3 */
    seq_t n4  = n2 * n2;          /* n^4 */
    seq_t f   = n4 - n3 - n2 + nn + 1; /* f(n) */

    seq_t s = floor_sqrt(f);           /* floor(sqrt(f)) */
    if (s * s == f)                    /* perfect square */
        return 0;

    seq_t t = (s + 1) * (s + 1);       /* next square */
    return t - f;                      /* a(n) */
}
