/*
 * A105693  a(n) = Fibonacci(2n+2)-2^n.
 * https://oeis.org/A105693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(2n+2) - 2^n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 998 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* fast doubling Fibonacci: returns F(n) in *fn and F(n+1) in *fn1 */
static void fib_fast_impl(idx_t n, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b, c, d;
    fib_fast_impl(n >> 1, &a, &b);          /* a = F(k), b = F(k+1) where k = n/2 */
    seq_t two_b          = b + b;           /* 2*F(k+1) */
    seq_t two_b_minus_a = two_b - a;       /* 2*F(k+1) - F(k) */
    c = a * two_b_minus_a;                 /* F(2k) = F(k)*(2*F(k+1)-F(k)) */
    d = a * a + b * b;                     /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n) {
    seq_t fn, fn1;
    fib_fast_impl(2 * n + 2, &fn, &fn1);   /* fn = Fibonacci(2n+2) */
    seq_t pow = pow2(n);
    return fn - pow;
}
