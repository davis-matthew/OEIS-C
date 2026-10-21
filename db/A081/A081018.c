/*
 * A081018  a(n) = (Lucas(4n+1)-1)/5, or Fibonacci(2n)*Fibonacci(2n+1), or
 *          A081017(n)/5.
 * https://oeis.org/A081018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2n) * Fibonacci(2n+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t f = 0; /* Fibonacci(0) */
    seq_t g = 1; /* Fibonacci(1) */

    /* Compute Fibonacci(2n) and Fibonacci(2n+1) by a linear scan */
    for (idx_t i = 0; i < 2 * n; ++i) {
        seq_t tmp = f + g;
        f = g;
        g = tmp;
    }

    /* f = F(2n), g = F(2n+1) */
    return f * g;  /* exact multiplication for arbitrary‑precision builds;
                      wrap‑around for native-width builds (acceptable) */
}
