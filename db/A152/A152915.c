/*
 * A152915  Exponacci (or exponential Fibonacci) numbers.
 * https://oeis.org/A152915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n ^ Fibonacci(n) (a(0) = 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

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
    if (n == 0) return 1;
    seq_t base = (seq_t)n;
    seq_t exp  = fib(n);
    return ipow(base, exp);
}
