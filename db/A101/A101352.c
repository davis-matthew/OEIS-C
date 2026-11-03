/*
 * A101352  Partial sums of A101351.
 * https://oeis.org/A101352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(n+2) + 2^(n+1) - n - 3
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-01)
 * Helpers:   fibonacci, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fibonacci.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t fib   = fibonacci(n + 2);        /* Fib(n+2) */
    seq_t p2    = pow2(n + 1);       /* 2^(n+1)   */
    seq_t sum   = fib + p2;
    sum -= (seq_t)n;
    sum -= (seq_t)3;
    return sum;
}
