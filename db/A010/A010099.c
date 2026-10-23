/*
 * A010099  a(n) = a(n-1)*a(n-2) with a(0)=1, a(1)=4.
 * https://oeis.org/A010099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*a(n-2) with a(0)=1, a(1)=4; Equivalent closed
 *            form: a(n) = 4^Fib(n), Fib(0)=0, Fib(1)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"     /* provides seq_t fib(idx_t n); */
#include "../utils/big_pow.h" /* provides seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)4;

    /* Compute the nth Fibonacci number as a big integer. */
    seq_t fib_n = fib(n);   /* fib(0)=0, fib(1)=1, fib(2)=1, ... */

    /* Return 4 raised to that exponent (exactly, using big integer arithmetic). */
    return big_pow((seq_t)4, fib_n);
}
