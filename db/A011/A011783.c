/*
 * A011783  Duplicate of A001519.
 * https://oeis.org/A011783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 15, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A011783 (Duplicate of A001519)
 *
 * a(0) = 1, a(1) = 1, a(n) = Fib(2n-1)  for n >= 2
 * where Fib(k) is the k-th Fibonacci number (Fib(0)=0, Fib(1)=1).
 */

#include <stddef.h>  /* for size_t, not used but harmless */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    /* Returns (Fib(n), Fib(n+1)) */
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);           /* a = Fib(k), b = Fib(k+1)  with k = n/2 */

    seq_t two_b = b + b;
    c = a * (two_b - a);                /* Fib(2k)   */
    d = a * a + b * b;                  /* Fib(2k+1) */

    if (n & 1) {                        /* n odd  */
        *fn  = d;
        *fn1 = c + d;
    } else {                            /* n even */
        *fn  = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        return 1;
    }

    /* index in Fibonacci sequence */
    seq_t k = (seq_t)n * 2 - 1;
    seq_t fn, fn1;
    fib_pair((idx_t)k, &fn, &fn1);     /* fn = Fib(k) */
    return fn;
}
