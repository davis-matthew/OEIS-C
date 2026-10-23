/*
 * A011369  a(n+1) = a(n) - F(n) if > 0, otherwise a(n) + F(n), where F() are
 *          Fibonacci numbers; a(0) = 0.
 * https://oeis.org/A011369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=0; For n ≥ 2: a(n) = 1 if n ≡ 2 (mod 3) a(n) = F(n-1)
 *            + 1 (Fibonacci) if n ≡ 0 (mod 3) a(n) = F(n) + 1 (Fibonacci) if
 *            n ≡ 1 (mod 3) where F(0)=0, F(1)=1, F(2)=1, …
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 48, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 0;

    idx_t r = n % 3;
    if (r == 2)
        return 1;

    if (r == 0)
        return fib(n - 1) + 1;
    else            /* r == 1 */
        return fib(n) + 1;
}
