/*
 * A117202  Binomial transform of n*F(n).
 * https://oeis.org/A117202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * F(2n-1) for n ≥ 1, a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here F(k) denotes the k‑th Fibonacci number (F(0)=0, F(1)=1). */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 0 */
        return (seq_t)0;

    /* Compute k = 2*n - 1.  For n ≥ 1 this is non‑negative and fits in idx_t. */
    idx_t k = n * 2 - 1;

    seq_t fib_val = fib(k);          /* Fibonacci(2n-1) */
    seq_t result  = (seq_t)n * fib_val;/* a(n) = n * F(2n-1) */

    return result;
}
