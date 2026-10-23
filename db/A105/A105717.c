/*
 * A105717  Smallest Fibonacci number that has 7 in the n-th position of the
 *          decimal representation.
 * https://oeis.org/A105717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest Fibonacci number that has a digit 7 at the n-th
 *            position from the right (0‑based)
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^n – the divisor used to isolate the n-th digit. */
    seq_t pow10_n = pow10(n);

    /* Current Fibonacci numbers: f = F_k, g = F_{k+1}. */
    seq_t f = 0; /* F_0 */
    seq_t g = 1; /* F_1 */

    while (1) {
        /* Extract the digit at position n of f.  The division truncates
           the lower n digits; %10 isolates the required digit. */
        seq_t digit = (f / pow10_n) % 10;
        if (digit == (seq_t)7)
            return f;

        /* Advance to the next Fibonacci number. */
        seq_t temp = f + g; /* F_{k+2} */
        f = g;              /* now F_{k+1} */
        g = temp;           /* now F_{k+2} */
    }
}
