/*
 * A081714  a(n) = F(n)*L(n+1) where F=Fibonacci and L=Lucas numbers.
 * https://oeis.org/A081714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n) * L(n+1) where F is the Fibonacci sequence (F(0)=0,
 *            F(1)=1) and L is the Lucas sequence (L(0)=2, L(1)=1). This
 *            implements the recurrence for F and L via the fast‑doubling
 *            method, giving O(log n) arithmetic per call
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the pair (F(n), F(n+1)).  The algorithm uses fast doubling. */
static void fib_pair(idx_t n, seq_t *f, seq_t *f1)
{
    if (n == 0) {
        *f  = 0;
        *f1 = 1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1), k = n/2   */
    seq_t two_b   = b + b;              /* 2*F(k+1)                         */
    seq_t two_b_a = two_b - a;          /* 2*F(k+1) - F(k)                  */
    c = a * two_b_a;                    /* F(2k)                             */
    d = a * a + b * b;                  /* F(2k+1)                           */
    if ((n & 1) == 0) {                 /* even n = 2k                       */
        *f  = c;
        *f1 = d;
    } else {                            /* odd n = 2k+1                      */
        *f  = d;
        *f1 = c + d;
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    seq_t f, f1;            /* f  = F(n),   f1 = F(n+1)       */
    fib_pair(n, &f, &f1);

    /* Lucas number L(n+1) = 2*F(n) + F(n+1) */
    seq_t l = f + f + f1;

    /* a(n) = F(n) * L(n+1) */
    return f * l;
}
