/*
 * A192243  0-sequence of reduction of Lucas sequence by x^2 -> x+1.
 * https://oeis.org/A192243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(2n‑1) - (n even ? 1 : 0)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided fib(idx_t n) helper from the utils library. */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute m = 2*n - 1 safely in idx_t. */
    idx_t m = (n << 1) - 1;

    /* Fibonacci number F(2n‑1). */
    seq_t f = fib(m);

    /* If n is even, subtract 1. */
    if ((n & 1) == 0)
    {
        f -= (seq_t)1;
    }

    return f;
}
