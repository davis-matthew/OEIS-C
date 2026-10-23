/*
 * A106517  Convolution of Fibonacci(n-1) and 3^n.
 * https://oeis.org/A106517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6·3ⁿ – Lucas(n+1)) / 5, where Lucas(n+1) = 2·F(n) +
 *            F(n+1) and (F(n),F(n+1)) are Fibonacci numbers via fast doubling
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void fib_pair(idx_t n, seq_t *f, seq_t *f_next)
{
    if (n == 0) {
        *f = 0;
        *f_next = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);           /* a = F(k), b = F(k+1) for k = n/2 */

    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);          /* F(2k)   */
    seq_t d = a * a + b * b;            /* F(2k+1) */

    if ((n & 1) == 0) {
        *f = c;
        *f_next = d;
    } else {
        *f = d;
        *f_next = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t f, f_next;
    fib_pair(n, &f, &f_next);           /* f = F(n), f_next = F(n+1) */

    /* Lucas(n+1) = 2·F(n) + F(n+1) */
    seq_t lucas = f + f + f_next;

    /* 3ⁿ via fast exponentiation */
    seq_t pow3 = 1;
    seq_t base = 3;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) pow3 = pow3 * base;
        base = base * base;
        exp >>= 1;
    }

    seq_t temp = 6 * pow3 - lucas;      /* 6·3ⁿ – Lucas(n+1) */
    seq_t result = temp / 5;            /* division is exact */
    return result;
}
