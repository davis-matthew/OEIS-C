/*
 * A094967  Right-hand neighbors of Fibonacci numbers in Stern's diatomic
 *          series.
 * https://oeis.org/A094967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n)*(1-(-1)^n)/2 + Fibonacci(n+1)*(1+(-1)^n)/2
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* For even n: a(n) = F_{n+1}; for odd n: a(n) = F_n */
    if ((n & 1U) == 0U)
        return fib(n + 1);
    else
        return fib(n);
}
