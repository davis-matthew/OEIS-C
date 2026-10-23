/*
 * A049654  a(n) = (F(8*n+1) - 1)/3 where F=A000045 (the Fibonacci sequence).
 * https://oeis.org/A049654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(8*n+1) - 1) / 3, where F = Fibonacci sequence
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    idx_t m = n * 8 + 1;          /* 8*n + 1  (fits in idx_t for tested ranges) */
    seq_t f = fib(m);            /* compute F(8*n+1) */
    f -= 1;
    f /= 3;
    return f;
}
