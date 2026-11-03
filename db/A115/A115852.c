/*
 * A115852  a(n) = ceiling((F(n)^4-1)^2 / (4*F(n)^4)) where F(n) = A000045(n)
 *          are the Fibonacci numbers.
 * https://oeis.org/A115852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (F(n)^4 + 2) / 4, where F(n) is the n-th Fibonacci number
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    seq_t f  = fib(n);          /* F(n) */
    seq_t f2 = f * f;           /* F(n)^2  */
    seq_t f4 = f2 * f2;         /* F(n)^4  */
    return (f4 + (seq_t)2) / (seq_t)4;
}
