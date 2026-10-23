/*
 * A024551  a(n) = floor(a(n-1)/(sqrt(5) - 2)) for n > 0 and a(0) = 1.
 * https://oeis.org/A024551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*Fibonacci(3*n+2)+1)/4 (which satisfies the recurrence
 *            a(n) = 5*a(n-1)-3*a(n-2)-a(n-3), with initial values a(0)=1,
 *            a(1)=4, a(2)=16)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle the base case n = 0 explicitly (the formula works for all n>=0). */
    idx_t k = 3 * n + 2;          /* k = 3*n + 2 */
    seq_t f = fib(k);             /* f = Fibonacci(k)  */
    /* Fibonacci(3*n+2) is always congruent to 1 mod 4,
       so the division by 4 is exact.  Compute safely without overflow. */
    seq_t t = (f - (seq_t)1) / (seq_t)4; /* (f-1)/4  */
    seq_t res = t * (seq_t)3 + (seq_t)1; /* 3*((f-1)/4)+1  */
    return res;
}
