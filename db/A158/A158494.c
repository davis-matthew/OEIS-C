/*
 * A158494  Boundary area of the T-square fractal.
 * https://oeis.org/A158494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2^n - 8 (n>3), a(1)=4, a(2)=24, a(3)=80
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 1) return 4U;
    if (n == 2) return 24U;
    if (n == 3) return 80U;

    /* Variables to hold the last three values of the sequence */
    seq_t prev2 = 24U;   /* a(n-2) when n>=4 */
    seq_t prev1 = 80U;   /* a(n-1) when n>=4 */
    seq_t cur   = 0U;

    /* pow2_curr holds 2^(k) for the current k in the loop.
       Start with 2^3, because we first need 2^4 when i == 4. */
    seq_t pow2_curr = 8U;  /* 2^3 */

    /* Iterate from n = 4 up to the requested n */
    for (idx_t i = 4; i <= n; ++i) {
        /* Advance to the next power of 2 (2^i) */
        pow2_curr <<= 1;                /* pow2_curr = 2^i */

        /* Recurrence: a(i) = 3*a(i-1) + 2^i - 8 */
        cur = (seq_t)3 * prev1 + pow2_curr - 8U;

        /* Shift the window for the next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
