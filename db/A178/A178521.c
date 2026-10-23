/*
 * A178521  The cost of all leaves in the Fibonacci tree of order n.
 * https://oeis.org/A178521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*F(n+1) - F(n) (F = Fibonacci numbers)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 39, u64 n <= 83, u128 n <= 174,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast‑doubling Fibonacci routine.
   Computes F(n) and F(n+1) into *fn and *fn1.  */
/* Assumes seq_t supports +, -, * and literals of small size. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1), k = n/2 */

    seq_t two_b = b * (seq_t)2;        /* 2*F(k+1) */
    seq_t c = a * (two_b - a);         /* F(2k)   = F(k) * [2*F(k+1) – F(k)] */
    seq_t d = a * a + b * b;           /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) {                /* n even */
        *fn  = c;
        *fn1 = d;
    } else {                           /* n odd  */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* Compute Fibonacci numbers F(n) and F(n+1). */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);

    /* Return n*F(n+1) - F(n).  Cast n to seq_t to keep the arithmetic
       in the target type. */
    return (seq_t)n * fn1 - fn;
}
