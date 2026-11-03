/*
 * A212262  a(n) = 3^n + Fibonacci(n).
 * https://oeis.org/A212262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + Fibonacci(n)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 995 (2026-11-01)
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fibonacci is computed via fast‑doubling in O(log n). */

#include <stdint.h>
#include "../utils/pow3.h"

static void fib_pair_local(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair_local(n >> 1, &a, &b);

    seq_t two_b             = b * 2;          /* 2 * b        */
    seq_t two_b_minus_a     = two_b - a;      /* 2*b - a      */
    seq_t c                 = a * two_b_minus_a; /* a*(2*b-a)  */
    seq_t d                 = a * a + b * b;   /* a^2 + b^2   */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t pow_val = pow3(n);        /* 3^n */
    seq_t fib_n, fib_n1;
    fib_pair_local(n, &fib_n, &fib_n1); /* fib_n = F_n */
    return pow_val + fib_n;
}
