/*
 * A205505  a(n) = Fibonacci(n*(n+1)) / Fibonacci(n).
 * https://oeis.org/A205505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n*(n+1)) / Fibonacci(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 66 (2026-11-01)
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

/* Computes A205505 term a(n) for n >= 1.
   Uses the utility function fib(idx_t) which returns Fibonacci(n) in
   the type seq_t.  The division is exact by definition of the sequence. */
seq_t a(idx_t n)
{
    /*  n*(n+1) fits safely into idx_t for all reasonable n (e.g. n <= 10000)   */
    idx_t numer_idx = (idx_t)n * (n + 1);

    seq_t fn  = fib(n);          /* Fibonacci(n)  */
    seq_t fN  = fib(numer_idx);  /* Fibonacci(n*(n+1)) */

    return fN / fn;  /* exact division */
}
