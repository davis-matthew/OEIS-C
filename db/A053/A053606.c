/*
 * A053606  a(n) = (Fibonacci(6*n+3) - 2)/4.
 * https://oeis.org/A053606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Fibonacci(6*n+3) - 2)/4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute k = 6*n + 3 (within idx_t range) */
    idx_t k = 6*n + 3;
    /* Retrieve Fibonacci(k) */
    seq_t f = fib(k);
    /* Apply the definition: (f - 2) / 4 */
    f -= 2;
    f /= 4;
    return f;
}
