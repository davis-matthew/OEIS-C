/*
 * A103434  a(n) = Sum_{i=1..n} Fibonacci(2i)^2.
 * https://oeis.org/A103434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Fibonacci(4n+2) - 2n - 1)/5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast‑doubling algorithm: compute F(n) and F(n+1) */
static void fib_doubling(idx_t n, seq_t *f, seq_t *f_next)
{
    if (n == 0) {
        *f = 0;
        *f_next = 1;
        return;
    }
    seq_t a, b;
    fib_doubling(n >> 1, &a, &b);
    seq_t two_b       = b << 1;           /* 2*b  */
    seq_t two_b_minus_a = two_b - a;       /* 2*b - a */
    seq_t c = a * two_b_minus_a;           /* a * (2*b - a)  */
    seq_t d = a * a + b * b;               /* a^2 + b^2      */
    if (n & 1) {
        *f = d;
        *f_next = c + d;
    } else {
        *f = c;
        *f_next = d;
    }
}

seq_t a(idx_t n)
{
    idx_t two_n = n << 1;                    /* 2n   */
    seq_t f, f_next;
    fib_doubling((two_n << 1) + 2, &f, &f_next); /* F(4n+2) */
    seq_t two_n_plus_one = (seq_t)two_n + 1;     /* 2n+1  */
    seq_t tmp = f - two_n_plus_one;
    return tmp / 5;
}
