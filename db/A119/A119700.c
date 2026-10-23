/*
 * A119700  a(n) = n*binomial(2*n, n)*Fibonacci(n)^2.
 * https://oeis.org/A119700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*binomial(2*n, n)*Fibonacci(n)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);
    seq_t t = b * 2;            /* t = 2*b */
    seq_t c = a * (t - a);      /* c = a * (2*b - a) */
    seq_t d = a * a + b * b;    /* d = a^2 + b^2 */
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
    /* Fibonacci(n) */
    seq_t fib_n, fib_n1;
    fib_pair(n, &fib_n, &fib_n1);

    /* Fibonacci(n)^2 */
    seq_t fib_sq = fib_n * fib_n;

    /* binomial(2n, n) computed multiplicatively */
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; ++i) {
        binom = binom * (seq_t)(n + i) / (seq_t)i;
    }

    /* a(n) = n * binom * fib_sq */
    seq_t result = (seq_t)n * binom;
    result = result * fib_sq;
    return result;
}
