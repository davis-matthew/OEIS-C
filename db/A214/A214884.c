/*
 * A214884  a(n) = Sum_{k=0..n} (-1)^k*F(k)*F(k+2), where F=A000045 (Fibonacci
 *          numbers).
 * https://oeis.org/A214884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = ((-1)^n * Fibonacci(2*n + 3) - 3*n - 2) / 5
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 21, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute F(2*n + 3) */
    seq_t fn = fib((idx_t)2 * n + (idx_t)3);

    /* (-1)^n * fn */
    seq_t term = (n & 1) ? -fn : fn;

    /* 3*n as seq_t */
    seq_t three_n = (seq_t)3 * (seq_t)n;

    /* numerator = term - 3*n - 2 */
    seq_t numerator = term - three_n - (seq_t)2;

    /* Divide by 5 (exact division) */
    return numerator / (seq_t)5;
}
