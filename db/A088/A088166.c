/*
 * A088166  Smallest integer divisible by Fibonacci(2n) such that the second
 *          partial quotient in the continued fraction expansion of a(n)/phi
 *          is 2 (phi is the golden ratio), n >= 2.
 * https://oeis.org/A088166
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(2n) * ceil(Lucas(2n)/2) (n >= 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 2; return 0 for lower indices. */
    if (n < 2)
        return 0;

    /* Compute Fibonacci numbers needed. */
    idx_t two_n   = n << 1;          /* 2*n */
    seq_t f2n      = fib(two_n);      /* Fib(2n)   */
    seq_t f2n_m1   = fib(two_n - 1);  /* Fib(2n-1) */
    seq_t f2n_p1   = fib(two_n + 1);  /* Fib(2n+1) */

    /* Lucas(2n) = Fib(2n-1) + Fib(2n+1) */
    seq_t lucas = f2n_m1 + f2n_p1;

    /* ceil(Lucas/2) = (lucas + 1) / 2   */
    seq_t ceil_half_lucas = (lucas + 1) / 2;

    /* Final result */
    return f2n * ceil_half_lucas;
}
