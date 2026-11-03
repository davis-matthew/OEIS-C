/*
 * A014166  Apply partial sum operator 4 times to Fibonacci numbers.
 * https://oeis.org/A014166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+8) - (n^3 +12*n^2 +59*n +126)/6
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 947 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

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

seq_t a(idx_t n) {
    seq_t fn = fib(n + 8);
    seq_t nn = (seq_t)n;
    seq_t poly = nn * nn * nn + 12 * nn * nn + 59 * nn + 126;
    seq_t div  = poly / 6;
    return fn - div;
}
