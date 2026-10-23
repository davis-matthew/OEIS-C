/*
 * A022115  Fibonacci sequence beginning 2, 11.
 * https://oeis.org/A022115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*F(n) + F(n+3)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the Fibonacci helper fib_pair from utils/fib_pair.h */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1, fn2, fn3, result;

    /* Compute F(n) and F(n+1) via fast doubling */
    fib_pair(n, &fn, &fn1);

    /* Compute F(n+2) and F(n+3) */
    fn2 = fn + fn1;          /* F(n+2) = F(n) + F(n+1) */
    fn3 = fn2 + fn1;         /* F(n+3) = F(n+2) + F(n+1) */

    /* a(n) = 8 * F(n) + F(n+3) */
    result = 8 * fn + fn3;

    return result;
}
