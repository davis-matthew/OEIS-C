/*
 * A078637  a(n) = rad(n(n+1)(n+2)), where rad(m) is the largest squarefree
 *          number dividing m (see A007947).
 * https://oeis.org/A078637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(n*(n+1)*(n+2)) Equivalent formulas: a(n) = rad(n) *
 *            rad(n+1) * rad(n+2) if n is odd; a(n) = rad(n/2) * rad(n+1) *
 *            rad(n/2 + 1) if n is even. (rad(x) is the squarefree kernel of
 *            x.)
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 1632, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset, unused */
    if (n & 1ULL)                  /* n is odd  */
        return rad(n) * rad(n + 1) * rad(n + 2);
    else                           /* n is even */
    {
        idx_t half = n >> 1;
        return rad(half) * rad(n + 1) * rad(half + 1);
    }
}
