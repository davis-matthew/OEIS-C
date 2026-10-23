/*
 * A016162  Expansion of 1/((1-5*x)*(1-8*x)).
 * https://oeis.org/A016162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 5^n with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) also equals (8^(n+1) - 5^(n+1)) / 3, but the recurrence
   provides the simplest iterative implementation. */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t acc = 1;          /* a(0) */
    seq_t pow5 = 1;         /* 5^0 */

    for (idx_t i = 1; i <= n; ++i) {
        pow5 *= 5;          /* pow5 = 5^i */
        acc  = acc * 8 + pow5;
    }
    return acc;
}
