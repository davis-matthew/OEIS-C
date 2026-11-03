/*
 * A027939  a(n) = T(2*n, n+3), T given by A027935.
 * https://oeis.org/A027939
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2*n+7) - (195 + 186*n + 90*n^2 + 35*n^3 +
 *            4*n^5)/15
 * Range:     u8 none, u16 n <= 6, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 1000 (2026-11-01)
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
    // The sequence is defined only for n >= 3, but we return 0 for smaller indices
    if (n < 3) return 0;

    // Compute Fibonacci(2*n + 7)
    idx_t fib_idx = 2 * n + 7;
    seq_t fn = fib(fib_idx);

    // Convert the index to seq_t for polynomial computation
    seq_t sn = (seq_t)n;

    // Compute the numerator of the polynomial
    seq_t poly = (seq_t)195;
    poly += (seq_t)186 * sn;
    poly += (seq_t)90 * sn * sn;
    poly += (seq_t)35 * sn * sn * sn;
    poly += (seq_t)4  * sn * sn * sn * sn * sn;

    // Divide by 15 (exact division)
    seq_t div = poly / (seq_t)15;

    // Result
    return fn - div;
}
