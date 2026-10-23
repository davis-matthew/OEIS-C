/*
 * A022345  Fibonacci sequence beginning 0, 11.
 * https://oeis.org/A022345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A022345:
   a(n) = 11 * F(n), with F(0) = 0, F(1) = 1 (Fibonacci numbers).
   The implementation uses the fast‑doubling algorithm to compute
   Fibonacci numbers in O(log n) time, works for any seq_t size
   (uint8_t … unsigned __int128) and arbitrary‑precision seq_t. */

#include "../utils/fib.h"   /* for fib_pair if needed, but we implement fib_fast directly */

/* Fast doubling algorithm: computes F(n) and F(n+1) */
static void fib_fast(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b, c, d;
    fib_fast(n >> 1, &a, &b);        /* k = n/2 => a=F(k), b=F(k+1) */

    /* c = F(2k)   = a * (2*b - a) */
    seq_t two_b        = b * (seq_t)2;
    seq_t two_b_minus_a = two_b - a;   /* 2*b >= a for Fibonacci numbers */
    c = a * two_b_minus_a;

    /* d = F(2k+1) = a*a + b*b */
    d = a * a + b * b;

    if (n & 1U) {                     /* n is odd */
        *fn  = d;
        *fn1 = c + d;
    } else {                          /* n is even */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_fast(n, &fn, &fn1);
    return fn * (seq_t)11;
}
