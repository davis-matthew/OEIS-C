/*
 * A186679  First differences of A116697.
 * https://oeis.org/A186679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n*Fibonacci(n+2) - (-1)^floor(n/2)
 * Range:     i8 n <= 9, i16 n <= 21, i32 n <= 44, i64 n <= 90, i128 n <= 182,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* sign1 = (-1)^n */
    int sign1 = (n & 1) ? -1 : 1;
    /* half = floor(n/2) */
    idx_t half = n >> 1;
    /* sign2 = (-1)^floor(n/2) */
    int sign2 = (half & 1) ? -1 : 1;
    /* Fibonacci term */
    seq_t fib_val = fib(n + 2);
    seq_t val = (seq_t)sign1 * fib_val - (seq_t)sign2;
    return val;
}
