/*
 * A180672  a(n) = Fibonacci(n+7) - Fibonacci(7).
 * https://oeis.org/A180672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+7) - Fibonacci(7) where Fibonacci(0)=0,
 *            Fibonacci(1)=1, so Fibonacci(7)=13. This implementation uses the
 *            provided efficient fib(idx_t) function
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* Compute Fibonacci(n+7) and subtract 13. */
    seq_t result = fib(n + 7);
    result -= (seq_t)13;
    return result;
}
