/*
 * A101353  a(n) = Sum_{k=0..n} (2^k + Fibonacci(k)).
 * https://oeis.org/A101353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (2^k + Fibonacci(k)) = 2^(n+1) +
 *            Fibonacci(n+2) - 2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   fib_fast, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/fib_fast.h"

seq_t a(idx_t n)
{
    /* compute 2^(n+1) */
    seq_t pow_val = pow2(n + 1);

    /* compute Fibonacci(n+2) using fast doubling */
    seq_t fib_val, fib_next;
    fib_fast(n + 2, &fib_val, &fib_next);

    /* result = 2^(n+1) + Fibonacci(n+2) - 2 */
    seq_t result = pow_val;
    result += fib_val;
    result -= (seq_t)2;

    return result;
}
