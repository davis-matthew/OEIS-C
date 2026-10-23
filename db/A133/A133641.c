/*
 * A133641  a(n) = 2*L(n) + L(n-1) - n, L(n) = n-th Lucas number A000204(n).
 * https://oeis.org/A133641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*L(n) + L(n-1) - n * = A000032(n+2) - n * =
 *            Fibonacci(n+3) + Fibonacci(n+1) - n (n > 1) * (L = Lucas
 *            numbers)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        /* the offset of the sequence */
        return (seq_t)1;
    }
    if (n == 0) {
        /* behaviour below the offset is irrelevant */
        return (seq_t)0;
    }
    /* use the existing utility for Fibonacci numbers */
    seq_t f1 = fib(n + 1);   /* Fibonacci(n+1) */
    seq_t f3 = fib(n + 3);   /* Fibonacci(n+3) */
    return f1 + f3 - (seq_t)n;
}
