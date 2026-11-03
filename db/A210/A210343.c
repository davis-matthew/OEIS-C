/*
 * A210343  a(n) = Fibonacci(n+1)^n.
 * https://oeis.org/A210343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fib(n+1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 70 (2026-11-01)
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
    seq_t base = fib(n + 1);
    seq_t exp  = (seq_t)n;
    return ipow(base, exp);
}
