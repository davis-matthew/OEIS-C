/*
 * A033813  Convolution of natural numbers n >= 1 with Lucas numbers L(k)
 *          (A000032) for k >= 3.
 * https://oeis.org/A033813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 18*(F(n+1)-1) + 11*F(n) – 7*n where F denotes the
 *            Fibonacci sequence (A000045)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term in the OEIS offset; values below 1 are
       undefined, return 0 as a safe default. */
    if (n < 1)
        return (seq_t)0;

    /* Fibonacci numbers F(n) and F(n+1).  fib() returns a seq_t. */
    seq_t fn  = fib(n);
    seq_t fn1 = fib(n + 1);

    /* a(n) = 18*F(n+1) – 18 + 11*F(n) – 7*n.  All operations are on
       seq_t, constants are cast to seq_t to avoid any type mismatch. */
    seq_t res = fn1 * (seq_t)18;
    res -= (seq_t)18;
    res += fn * (seq_t)11;
    res -= (seq_t)n * (seq_t)7;

    return res;
}
