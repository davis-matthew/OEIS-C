/*
 * A156088  Alternating sum of the squares of the first n even-indexed
 *          Fibonacci numbers.
 * https://oeis.org/A156088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * F(2*n) * F(2*n+2) / 3
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 23, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Local fast‐doubling helper: set fn = F(n), fn1 = F(n+1). */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);

    /* c = F(2k)  = F(k) * (2*F(k+1) – F(k)) */
    seq_t two_b = b + b;           /* 2*F(k+1) */
    seq_t c = a * (two_b - a);

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    seq_t d = a * a + b * b;

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
    seq_t f2n, f2n1;
    idx_t two_n = n << 1;           /* 2*n */
    fib_pair(two_n, &f2n, &f2n1);   /* f2n = F(2n), f2n1 = F(2n+1) */

    seq_t f2n2 = f2n + f2n1;          /* F(2n+2) */
    seq_t prod = f2n * f2n2;          /* product */
    seq_t res  = prod / 3;           /* division is exact */

    if ((n & 1) != 0)                /* apply (-1)^n  */
        res = -res;

    return res;
}
