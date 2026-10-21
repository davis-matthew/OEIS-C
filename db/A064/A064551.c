/*
 * A064551  Ado [Simone Caramel]'s function: a(0) = 1, a(n) = a(n-1) +
 *          2*(Fibonacci(n+1)-n), n > 0.
 * https://oeis.org/A064551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*F(n+3) - n*(n+1) - 3 (F = Fibonacci, F(0)=0, F(1)=1)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* compute F(n+3) */
    seq_t fn3 = fib(n + 3);

    /* convert n to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* n * (n + 1) */
    seq_t nnp1 = n_seq * (n_seq + 1);

    /* compute 2 * F(n+3) - n*(n+1) - 3 */
    seq_t result = fn3 + fn3;   /* 2 * F(n+3) */
    result -= nnp1;             /* subtract n*(n+1) */
    result -= 3;                /* subtract 3 */

    return result;
}
