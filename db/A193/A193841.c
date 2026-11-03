/*
 * A193841  Erroneous version of A227590.
 * https://oeis.org/A193841
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = F_{n+2} - 1 for 2 ≤ n ≤ 5, a(n) = F_n + 2 for n > 5 (F_k
 *            denotes the k‑th Fibonacci number)
 * Range:     i8 n <= 8, i16 n <= 8, i32 n <= 8, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 8 (2026-11-01)
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

seq_t a(idx_t n)
{
    if (n <= 5)
        return fib(n + 2) - 1;
    else
        return fib(n) + 2;
}
