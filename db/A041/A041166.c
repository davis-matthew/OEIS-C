/*
 * A041166  Numerators of continued fraction convergents to sqrt(93).
 * https://oeis.org/A041166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recurrence for A041166:
   a(0) = 9.
   For n ≥ 1: a(n) = c_n * a(n-1) + a(n-2),
   where the continued‑fraction coefficients c_n of √93 repeat
   the sequence [1,1,1,4,6,4,1,1,1,18] (period length 10)
   starting with n = 1. */
seq_t a(idx_t n)
{
    static const seq_t period[10] = {1,1,1,4,6,4,1,1,1,18};
    seq_t prevprev = 1;   /* a(-1)   */
    seq_t prev     = 9;   /* a(0)    */

    if (n == 0) return prev;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t coeff = period[(i - 1) % 10];
        seq_t curr  = coeff * prev + prevprev;
        prevprev = prev;
        prev    = curr;
    }
    return prev;
}
