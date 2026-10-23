/*
 * A008466  a(n) = 2^n - Fibonacci(n+2).
 * https://oeis.org/A008466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^n - Fibonacci(n+2) (offset 0)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

/* Fast doubling method for Fibonacci numbers.
   Computes F(n) and F(n+1). */
static void fib_fast(idx_t n, seq_t *Fn, seq_t *Fn1) {
    if (n == 0) {
        *Fn  = 0;
        *Fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_fast(n >> 1, &a, &b);           /* a = F(k), b = F(k+1), k = floor(n/2) */

    seq_t two_b = b * 2;                /* 2*F(k+1) */
    seq_t c = a * (two_b - a);          /* F(2k)   */
    seq_t d = a * a + b * b;            /* F(2k+1) */

    if ((n & 1) == 0) {
        *Fn  = c;
        *Fn1 = d;
    } else {
        *Fn  = d;
        *Fn1 = c + d;
    }
}

seq_t a(idx_t n) {
    /* 2^n */
    seq_t pow2n = pow2(n);

    /* Fibonacci(n+2) */
    seq_t fibn2, unused;
    fib_fast(n + 2, &fibn2, &unused);

    /* Difference */
    return pow2n - fibn2;
}
