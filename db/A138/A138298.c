/*
 * A138298  First differences of A137976 after having added two leading ones.
 * https://oeis.org/A138298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A138298
   Formula:
     a(0) = 0
     a(1) = 2
     a(2n)   = 2 * Fib(5n)
     a(2n+1) = Fib(5n+3)
   where Fib(0)=0, Fib(1)=1.  The Fibonacci numbers are
   computed by fast doubling, which runs in O(log n) time. */

#include <stddef.h>

/* fast doubling: computes Fn and Fn+1 and stores them in *f and *f_next.
   n is a non‑negative idx_t.  The result assumes the usual Fibonacci
   definition Fib(0)=0, Fib(1)=1. */
static void fib_pair(idx_t n, seq_t *f, seq_t *f_next)
{
    if (n == 0) {
        *f = 0;
        *f_next = 1;
        return;
    }

    seq_t a, b;                /* a = F(k),  b = F(k+1)  with k = n/2 */
    fib_pair(n >> 1, &a, &b);

    seq_t two_b_minus_a = (b << 1) - a;        /* 2*b - a */
    seq_t c = a * two_b_minus_a;              /* F(2k)   */
    seq_t d = a * a + b * b;                  /* F(2k+1) */

    if ((n & 1) == 0) {
        *f = c;
        *f_next = d;
    } else {
        *f = d;
        *f_next = c + d;
    }
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* a(0) = 0 */

    if (n == 1)
        return 2;               /* a(1) = 2 */

    if ((n & 1U) == 0) {        /* n even: n = 2k  */
        idx_t k = n >> 1;              /* k = n/2 */
        idx_t fib_idx = 5 * k;         /* need Fib(5k) */
        seq_t f, f_next;
        fib_pair(fib_idx, &f, &f_next);
        return f + f;                  /* 2 * Fib(5k) */
    } else {                       /* n odd: n = 2k+1  */
        idx_t k = (n - 1) >> 1;       /* k = (n-1)/2 */
        idx_t fib_idx = 5 * k + 3;    /* need Fib(5k+3) */
        seq_t f, f_next;
        fib_pair(fib_idx, &f, &f_next);
        return f;                    /* Fib(5k+3) */
    }
}
