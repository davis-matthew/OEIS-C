/*
 * A132607  X-values of solutions to the equation X*(X + 1) - 11*Y^2 = 0.
 * https://oeis.org/A132607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 398*a(n-1) - a(n-2) + 198; a(0)=0, a(1)=99
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 99;

    seq_t prev2 = 0;   /* a(n-2)  */
    seq_t prev1 = 99;  /* a(n-1)  */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 398 * prev1 - prev2 + 198;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
