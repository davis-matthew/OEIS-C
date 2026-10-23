/*
 * A180032  Eight white queens and one red queen on a 3 X 3 chessboard. G.f.:
 *          (1+x)/(1-5*x-7*x^2).
 * https://oeis.org/A180032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 7*a(n-2) with a(0)=1, a(1)=6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return 1;
    if (n == 1)
        return 6;

    /* Iteratively compute the recurrence */
    seq_t a0 = 1;   /* a(0) */
    seq_t a1 = 6;   /* a(1) */
    seq_t ai;      /* a(i) for i >= 2 */

    for (idx_t i = 2; i <= n; ++i) {
        ai = 5 * a1 + 7 * a0;  /* a(i) = 5*a(i-1) + 7*a(i-2) */
        a0 = a1;               /* shift: a(i-2) <- a(i-1) */
        a1 = ai;               /* shift: a(i-1) <- a(i)   */
    }

    return ai;
}
