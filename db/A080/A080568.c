/*
 * A080568  Sum of the Fibonacci numbers A000045 and the factorials A000142.
 * https://oeis.org/A080568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = fib(n) + n!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 96 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

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

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    seq_t f;
    /* compute Fibonacci F(n) */
    if (n == 0) {
        f = 0;
    } else if (n == 1) {
        f = 1;
    } else {
        seq_t f0 = 0, f1 = 1, tmp = 0;
        for (idx_t k = 2; k <= n; ++k) {
            tmp = f1;
            f1 = f0 + f1;
            f0 = tmp;
        }
        f = f1;
    }

    /* compute factorial n! */
    seq_t fact = 1;
    for (idx_t k = 2; k <= n; ++k) {
        fact = fact * (seq_t)k;
    }

    return f + fact;
}
