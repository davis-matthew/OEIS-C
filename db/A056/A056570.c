/*
 * A056570  Third power of Fibonacci numbers (A000045).
 * https://oeis.org/A056570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Fibonacci(n))^3 where Fibonacci(0)=0, Fibonacci(1)=1 and
 *            Fibonacci(n)=Fibonacci(n-1)+Fibonacci(n-2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 171 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t if needed (seq_t may be bigger) */

/* Fast doubling method to compute (F(n),F(n+1)) */
static void fib_fast_doubling(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b;
    fib_fast_doubling(n >> 1, &a, &b);

    seq_t two_b = b + b;          /* 2 * F(k+1)  */
    seq_t c     = a * (two_b - a);/* F(2k)      */
    seq_t d     = a * a + b * b;  /* F(2k+1)    */

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
    fib_fast_doubling(n, &fn, &fn1);              /* fn = F(n) */
    return fn * fn * fn;                         /* cube of the Fibonacci number */
}
