/*
 * A068397  a(n) = Lucas(n) + (-1)^n + 1.
 * https://oeis.org/A068397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A068397: a(n) = Lucas(n) + (-1)^n + 1
 *
 *  Lucas numbers L(n) satisfy L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2).
 *  For n>=1 the sequence is:
 *      a(n) = L(n)          if n is odd
 *      a(n) = L(n)+2        if n is even
 *
 *  The function computes L(n) using the fast‑doubling algorithm for
 *  Fibonacci numbers and the identity L(n) = 2*F(n+1)-F(n).
 *  Complexity: O(log n) multiplications of seq_t values.
 */

#include <stdbool.h> /* for bool, not essential but harmless */

static void fib_fast_doubling(idx_t n, seq_t *fn, seq_t *fn1)
{
    /* Returns (F(n), F(n+1)) in fn and fn1, using recursion. */
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;              /* a=F(k), b=F(k+1) where k=n/2 */
    fib_fast_doubling(n >> 1, &a, &b);

    /* c = F(2k)   = F(k) * (2*F(k+1) - F(k)) */
    seq_t two_b = b + b;                     /* 2*F(k+1) */
    seq_t two_b_minus_a = two_b - a;         /* 2*F(k+1) - F(k) */
    seq_t c = a * two_b_minus_a;             /* F(2k) */

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    seq_t d = a * a + b * b;                 /* F(2k+1) */

    if ((n & 1) == 0) {                      /* n even   */
        *fn  = c;
        *fn1 = d;
    } else {                                 /* n odd    */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* n >= 1 according to the OEIS offset */
    seq_t fn, fn1;
    fib_fast_doubling(n, &fn, &fn1);          /* fn  = F(n)   */
                                               /* fn1 = F(n+1) */
    seq_t lucas = fn1 * 2 - fn;               /* L(n) = 2*F(n+1) - F(n) */

    if (n & 1) {                              /* odd n:  a(n) = L(n) */
        return lucas;
    } else {                                  /* even n: a(n) = L(n)+2 */
        return lucas + 2;
    }
}
