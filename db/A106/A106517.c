/*
 * A106517  Convolution of Fibonacci(n-1) and 3^n.
 * https://oeis.org/A106517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6·3ⁿ – Lucas(n+1)) / 5, where Lucas(n+1) = 2·F(n) +
 *            F(n+1) and (F(n),F(n+1)) are Fibonacci numbers via fast doubling
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 746 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

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

seq_t a_wide(idx_t n)
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

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
