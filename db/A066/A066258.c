/*
 * A066258  a(n) = Fibonacci(n)^2 * Fibonacci(n+1).
 * https://oeis.org/A066258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n)^2 * Fibonacci(n+1) The function computes
 *            Fibonacci numbers using the fast‑doubling recurrence. It works
 *            for any build where seq_t supports the standard arithmetic
 *            operators (+, -, *)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling algorithm for Fibonacci numbers.
   After execution, fn  = F(n),   fn1 = F(n+1)   (0‑based).  */
static void fib_pair_rec(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t a, b;          /* a = F(k), b = F(k+1)  */
    fib_pair_rec(n >> 1, &a, &b);

    seq_t two_b    = b * 2;          /* 2*F(k+1)   */
    seq_t c        = a * (two_b - a);/* F(2k) = F(k)*(2*F(k+1)-F(k)) */
    seq_t d        = a * a + b * b; /* F(2k+1) = F(k)^2 + F(k+1)^2 */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Public sequence function as required by the harness.  */
seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair_rec(n, &fn, &fn1);
    return fn * fn * fn1;
}
