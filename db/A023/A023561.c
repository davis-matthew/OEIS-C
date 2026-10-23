/*
 * A023561  Convolution of A023531 and (F(2), F(3), F(4), ...).
 * https://oeis.org/A023561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Formula: a(n) = sum_{k=1}^{tmax} F(n - k(k+3)/2 + 2),
 *  where k(k+3)/2 <= n and F(m) is the standard Fibonacci number
 *  with F(0)=0, F(1)=1.  The sequence A023531 consists of 1's
 *  at positions n = k(k+3)/2 (k ≥ 1); all other terms are 0.
 */

#include <stdint.h>

/* Fast doubling Fibonacci algorithm.
 * Returns (F(n), F(n+1)) in *fn and *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    idx_t half = n >> 1;
    seq_t a, b;
    fib_pair(half, &a, &b);

    seq_t two_b    = b + b;          /* 2*F(k+1)   */
    seq_t c        = a * (two_b - a);/* F(2k)      */
    seq_t d        = a * a + b * b;  /* F(2k+1)    */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Return F(n) */
static seq_t fib_fast(idx_t n)
{
    seq_t f, f1;
    fib_pair(n, &f, &f1);
    return f;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* Loop over all k such that k(k+3)/2 <= n */
    for (idx_t k = 1;; ++k) {
        __uint128_t tmp = (__uint128_t)k * (k + 3) / 2;
        if (tmp > n)
            break;
        idx_t s    = (idx_t)tmp;          /* k(k+3)/2   */
        idx_t term = n - s + 2;           /* index for Fib   */
        sum += fib_fast(term);
    }
    return sum;
}
