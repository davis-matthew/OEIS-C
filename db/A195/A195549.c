/*
 * A195549  Hypotenuses of primitive Pythagorean triples in A195547 and
 *          A195548.
 * https://oeis.org/A195549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (F(n)^2 + F(n+1)^2) / (2 - ((n+2)^2 mod 3))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where F(k) is the k‑th Fibonacci number (F(0)=0, F(1)=1). */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 otherwise. */
    if (n < 1)
        return 0;

    /* Compute the required Fibonacci terms. */
    seq_t fn  = fib(n);       /* F(n)  */
    seq_t fn1 = fib(n + 1);   /* F(n+1) */

    /* Numerator: F(n)^2 + F(n+1)^2. */
    seq_t numer = fn * fn + fn1 * fn1;

    /* Denominator is 2 when n ≡ 1 (mod 3), otherwise 1. */
    if ((n % 3) == 1)
        numer /= 2;

    return numer;
}
