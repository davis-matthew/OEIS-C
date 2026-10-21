/*
 * A051124  a(n) = Fibonacci(n) XOR Fibonacci(n+1).
 * https://oeis.org/A051124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Fibonacci(n) XOR Fibonacci(n+1)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements fast doubling Fibonacci to compute both terms efficiently. */
#include <stdint.h>

static void fib_fast(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_fast(n >> 1, &a, &b);       /* a = F(k), b = F(k+1), k = n/2 */
    seq_t two_b_minus_a = (b * ((seq_t)2)) - a;  /* 2*F(k+1) - F(k) */
    seq_t c = a * two_b_minus_a;    /* F(2k)     */
    seq_t d = (a * a) + (b * b);    /* F(2k+1)   */
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
    seq_t fn, fn1;
    fib_fast(n, &fn, &fn1);
    return fn ^ fn1;   /* XOR of the two Fibonacci terms */
}
