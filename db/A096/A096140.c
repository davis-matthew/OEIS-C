/*
 * A096140  a(n) = sum of n Fibonacci numbers starting from F(n).
 * https://oeis.org/A096140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+1)-Fibonacci(n+1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    /* Compute F(2*n+1) and F(n+1) using fast doubling */
    seq_t f2n1, f2n2, fn, fn2;
    idx_t m = 2 * n + 1;
    fib_pair(m, &f2n1, &f2n2);          /* f2n1 = F(2*n+1) */

    idx_t t = n + 1;
    fib_pair(t, &fn, &fn2);             /* fn = F(n+1) */

    /* a(n) = F(2*n+1) - F(n+1) */
    return f2n1 - fn;
}
