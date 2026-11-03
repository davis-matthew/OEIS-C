/*
 * A122491  a(n) = n * Fibonacci(n) - Sum_{i=0..n} Fibonacci(i).
 * https://oeis.org/A122491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Fibonacci(n) - Fibonacci(n+2) + 1
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 40, u64 n <= 84, u128 n <= 175,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 968 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses the identity a(n) = n*Fib(n) - (Fib(n+2)-1)   */
/* Hence a(n) = n*Fib(n) + 1 - Fib(n+2).  */

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
    /* Compute Fibonacci numbers needed */
    seq_t f_n   = fib(n);           /* Fib(n)   */
    idx_t n_plus2 = n + 2;          /* n + 2    */
    seq_t f_np2 = fib(n_plus2);    /* Fib(n+2) */

    /* a(n) = n*Fib(n) + 1 - Fib(n+2)  */
    seq_t res = (seq_t)n * f_n;    /* n * Fib(n)  */
    res += 1;                      /* + 1         */
    res -= f_np2;                  /* - Fib(n+2)  */

    return res;
}
