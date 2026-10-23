/*
 * A119587  2^n + 1 - 2*Fibonacci(n+1).
 * https://oeis.org/A119587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 1 - 2*Fib(n+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   fib, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast exponentiation and Fibonacci from the utils library. */
#include "../utils/pow2.h"
#include "../utils/fib.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t pow_val  = pow2(n);          /* 2^n   */
    seq_t fib_val  = fib(n + 1);       /* Fib(n+1) */
    seq_t twofib   = mul_small(fib_val, 2); /* 2*Fib(n+1) */

    seq_t result = pow_val + (seq_t)1 - twofib;
    return result;
}
