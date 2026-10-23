/*
 * A103646  G.f.: 9*(3x+1)/(1+2x-6x^2-27x^3).
 * https://oeis.org/A103646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 9, a(1) = 9, a(2) = 36; Recurrence: a(n+3) = -2*a(n+2) +
 *            6*a(n+1) + 27*a(n) for n ≥ 0 (OEIS A103646)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 0) return 9;
    if (n == 1) return 9;
    if (n == 2) return 36;

    /* Use a sliding window of the last three values */
    seq_t prev3 = 9;   /* a(n-3)   */
    seq_t prev2 = 9;   /* a(n-2)   */
    seq_t prev1 = 36;  /* a(n-1)   */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(i) = 27*a(i-3) + 6*a(i-2) - 2*a(i-1) */
        cur = 27 * prev3 + 6 * prev2 - 2 * prev1;
        /* Shift the window for the next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
