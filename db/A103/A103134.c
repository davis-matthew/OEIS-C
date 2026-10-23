/*
 * A103134  a(n) = Fibonacci(6n+4).
 * https://oeis.org/A103134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(6n+4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive fast–doubling algorithm.
   For a given n it returns (F(n), F(n+1)).
   All arithmetic is performed on seq_t so it works
   with normal unsigned integer types or with an
   arbitrary‑precision integer class. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn   = 0;
        *fn1  = 1;
        return;
    }
    idx_t half = n >> 1;
    seq_t a, b;
    fib_pair(half, &a, &b);          /* a = F(half), b = F(half+1) */

    seq_t t1 = b + b;                 /* 2*b */
    seq_t c  = a * (t1 - a);          /* F(2k)  */
    seq_t d  = a * a + b * b;         /* F(2k+1) */

    if ((n & 1) == 0) {
        *fn  = c;                     /* F(2k)   */
        *fn1 = d;                     /* F(2k+1) */
    } else {
        *fn  = d;                     /* F(2k+1) */
        *fn1 = c + d;                 /* F(2k+2) */
    }
}

seq_t a(idx_t n)
{
    idx_t m = 6 * n + 4;              /* index of the Fibonacci number */
    seq_t fn, fn1;
    fib_pair(m, &fn, &fn1);
    return fn;                        /* Fib(6n+4) */
}
