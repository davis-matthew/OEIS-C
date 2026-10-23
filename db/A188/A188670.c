/*
 * A188670  A quasi-periodic solution to Hofstadter's Q recurrence.
 * https://oeis.org/A188670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3n) = 3, a(3n+1) = 6, a(3n+2) = Fib(n+5)
 * Range:     u8 n <= 28, u16 n <= 61, u32 n <= 130, u64 n <= 268,
 *            u128 n <= 547, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    switch (n % 3) {
        case 0:
            return (seq_t)3;
        case 1:
            return (seq_t)6;
        case 2: {
            idx_t k = (n - 2) / 3;   /* index for Fibonacci: k+5 */
            return fib(k + 5);
        }
    }
    /* unreachable but keeps compiler happy */
    return (seq_t)0;
}
