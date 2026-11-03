/*
 * A192475  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x)=1+x^(n+1)+x^(2n).
 * https://oeis.org/A192475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fib(2n) + Fib(n+1) (Fib = Fibonacci numbers from A000045,
 *            Fib(0)=0, Fib(1)=1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 28 (2026-11-01)
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
    /* Compute Fib(2n) and Fib(n+1) and return their sum. */
    seq_t f2n = fib(n << 1);   /* 2n fits in idx_t for the intended input range */
    seq_t fnp1 = fib((n + 1) & ((idx_t)-1));
    return f2n + fnp1;
}
