/*
 * A117152  Sum of product of Fibonacci and triangular numbers.
 * https://oeis.org/A117152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,2)*F(n+2) - n*F(n+3) + F(n+5) - 5
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t fn2 = fib(n + 2);
    seq_t fn3 = fib(n + 3);
    seq_t fn5 = fib(n + 5);

    seq_t binom = ((seq_t)n * ((seq_t)n - 1)) / 2;

    seq_t term1 = binom * fn2;
    seq_t result = term1 + fn5;

    result -= ((seq_t)n) * fn3;
    result -= (seq_t)5;

    return result;
}
