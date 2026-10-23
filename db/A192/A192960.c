/*
 * A192960  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Outcome: this code implements the OEIS formula
   a(n) = 2*Fibonacci(n+4) - (2*n + 5)   (valid for n ≥ 0)  */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* 2 × Fibonacci(n+4) */
    seq_t fn = fib(n + 4u);
    seq_t res = fn + fn;               // multiply by 2

    /* (2*n + 5) */
    seq_t sub = (seq_t)2 * (seq_t)n + (seq_t)5u;

    res -= sub;
    return res;
}
