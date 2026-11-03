/*
 * A013980  Number of commutative elements in Coxeter group F_n.
 * https://oeis.org/A013980
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n-1) + 5*F(3*n-3) - 2*F(2*n-1) - 2*F(2*n-3) + F(n) - 1
 *            * - 5* Σ_{k=2..n-1} F(3*k-4) * C(n-k) * where C is Catalan
 *            numbers (A000108) and F is Fibonacci numbers (A000045)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 24 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

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
    if (n < 3) return 0;   /* Undefined for n < 3; return 0 */
    seq_t result = catalan(n - 1);

    idx_t i3n = 3*n - 3;
    idx_t i2n1 = 2*n - 1;
    idx_t i2n3 = 2*n - 3;

    /* add terms involving Fibonacci numbers */
    result += 5 * fib(i3n);
    result -= 2 * fib(i2n1);
    result -= 2 * fib(i2n3);
    result += fib(n);
    result -= 1;

    /* sum over k */
    seq_t sum = 0;
    for (idx_t k = 2; k <= n - 1; ++k) {
        seq_t fk = fib(3*k - 4);
        seq_t ck = catalan(n - k);
        sum += fk * ck;
    }

    result -= 5 * sum;
    return result;
}
