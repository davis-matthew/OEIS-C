/*
 * A105995  Fibonacci sequence (mod 14).
 * https://oeis.org/A105995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 14 Recurrence: a(0)=0, a(1)=1,
 *            a(n)=a(n-1)+a(n-2) (mod 14)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   fib_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_mod.h"

seq_t a(idx_t n)
{
    /* The Fibonacci number modulo 14 can be computed efficiently
       using the provided fib_mod helper. */
    return fib_mod(n, 14);
}
