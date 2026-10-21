/*
 * A094586  Central numbers of the triangle T of all positive differences of
 *          distinct Fibonacci numbers.
 * https://oeis.org/A094586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2n+2) - Fibonacci(n+2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1) {
    if (n == (idx_t)0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);           /* a = F(k), b = F(k+1), k = n/2 */
    seq_t two_b = b << 1;              /* 2*b */
    seq_t c = a * (two_b - a);          /* F(2k)   = F(k) * (2*F(k+1) – F(k)) */
    seq_t d = a * a + b * b;            /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    if ((n & (idx_t)1) == 0) {          /* even n */
        *fn  = c;
        *fn1 = d;
    } else {                            /* odd n  */
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Fibonacci of a non‑negative index */
static seq_t fib(idx_t k) {
    seq_t fn, fn1;
    fib_pair(k, &fn, &fn1);
    return fn;
}

seq_t a(idx_t n) {
    /* indices use the same type as n */
    idx_t two_n_plus_2 = n * (idx_t)2 + (idx_t)2;
    idx_t n_plus_2     = n + (idx_t)2;
    seq_t f1 = fib(two_n_plus_2);
    seq_t f2 = fib(n_plus_2);
    return f1 - f2;
}
