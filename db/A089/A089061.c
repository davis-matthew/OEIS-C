/*
 * A089061  a(0) = 5, a(1) = 7; for n>1, a(n) = a(n-1) + a(n-2) - (2*n-2).
 * https://oeis.org/A089061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(n+1) + 2*n + 4
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided Fibonacci routine (fib.h) */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n + 1);           /* Fib(n+1)          */
    seq_t two_n = (seq_t)n * (seq_t)2;   /* 2 * n              */
    return f + two_n + (seq_t)4;    /* Fib(n+1) + 2n + 4  */
}
