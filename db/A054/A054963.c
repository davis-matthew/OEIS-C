/*
 * A054963  Number of cells in the first column of all directed column-convex
 *          polyominoes of area n+1.
 * https://oeis.org/A054963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(2n) + Fib(2n+2) - n (where Fib(0)=0, Fib(1)=1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib_doubling
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_doubling.h"

seq_t a(idx_t n)
{
    /* First compute Fib(n) and Fib(n+1) */
    seq_t fn, fn1;
    fib_doubling(n, &fn, &fn1);

    seq_t two_fn1 = fn1 + fn1;          /* 2*Fib(n+1) */
    seq_t fib_2n  = fn * (two_fn1 - fn); /* Fib(2n)   */
    seq_t fib_2n1 = fn1 * fn1 + fn * fn;/* Fib(2n+1) */
    seq_t fib_2n2 = fib_2n1 + fib_2n;   /* Fib(2n+2) */

    seq_t res = fib_2n + fib_2n2 - (seq_t)n;
    return res;
}
