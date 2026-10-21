/*
 * A056571  Fourth power of Fibonacci numbers A000045.
 * https://oeis.org/A056571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)^4 where F(0) = 0, F(1) = 1 are the Fibonacci numbers
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast‑doubling algorithm to compute F(n) exactly. */
static inline void fib_pair_rec(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair_rec(n >> 1, &a, &b);                     /* a = F(k), b = F(k+1) */
    seq_t c = a * ((b << 1) - a);                    /* F(2k)   = a*(2*b - a) */
    seq_t d = a * a + b * b;                         /* F(2k+1) = a^2 + b^2 */
    if ((n & 1) == 0) {                              /* even n   */
        *fn  = c;
        *fn1 = d;
    } else {                                          /* odd n    */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* a(0) = 0 by definition */
    seq_t fn, fn1;
    fib_pair_rec(n, &fn, &fn1);          /* fn = F(n) */
    seq_t sq  = fn * fn;                 /* F(n)^2   (exact in seq_t) */
    seq_t sq2 = sq * sq;                 /* F(n)^4   */
    return sq2;
}
