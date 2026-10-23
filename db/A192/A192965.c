/*
 * A192965  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+4) + 3*Fibonacci(n+2) - (n^2 + 3*n + 6)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    seq_t fn4 = fib(n + 4);
    seq_t fn2 = fib(n + 2);
    seq_t quad = (seq_t)n * (seq_t)n + (seq_t)3 * (seq_t)n + (seq_t)6;
    seq_t res = fn4 + (seq_t)3 * fn2 - quad;
    return res;
}
