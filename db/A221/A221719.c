/*
 * A221719  a(n) = 3*2^n - Fibonacci(n+3) - 1.
 * https://oeis.org/A221719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n - Fibonacci(n+3) - 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 992 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    seq_t pow2n = pow2(n);                // 2^n
    seq_t result = pow2n + pow2n;         // 2*2^n
    result += pow2n;                      // 3*2^n
    seq_t fib_term = fib(n + 3);          // Fibonacci(n+3)
    result -= fib_term;
    result -= (seq_t)1;
    return result;
}
