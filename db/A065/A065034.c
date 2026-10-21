/*
 * A065034  a(n) = Lucas(2*n) + 1.
 * https://oeis.org/A065034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2*n) + 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Lucas(2n) = Lucas(n)^2 - 2*(-1)^n)       */
/* Uses fast doubling Fibonacci to compute Lucas(n) efficiently. */

#include <stdint.h>
#include <stdbool.h>

/* Fibonacci pair: fn = F(n), fn1 = F(n+1) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;   /* F(0) */
    seq_t b = 1;   /* F(1) */

    /* Find number of bits in n */
    idx_t temp = n;
    unsigned int bits = 0;
    while (temp) { temp >>= 1; bits++; }

    for (int i = bits - 1; i >= 0; --i) {
        /* c = F(2k) = F(k) * (2*F(k+1) - F(k)) */
        seq_t c = a * (((b << 1) - a));
        /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
        seq_t d = a * a + b * b;

        if ((n >> i) & 1) {
            a = d;          /* F(2k+1) */
            b = c + d;      /* F(2k+2) = F(2k) + F(2k+1) */
        } else {
            a = c;          /* F(2k)   */
            b = d;          /* F(2k+1) */
        }
    }

    *fn  = a;  /* F(n)   */
    *fn1 = b;  /* F(n+1) */
}

seq_t a(idx_t n)
{
    seq_t f, f1;     /* Fibonacci numbers */
    fib_pair(n, &f, &f1);

    /* Lucas(n) = 2*F(n+1) - F(n) */
    seq_t lucas_n = (f1 << 1) - f;

    /* Lucas(2n) = Lucas(n)^2 - 2*(-1)^n */
    seq_t lucas_2n;
    if (n & 1) {           /* n odd: (-1)^n = -1, add 2 */
        lucas_2n = lucas_n * lucas_n + 2;
    } else {               /* n even: (-1)^n = +1, subtract 2 */
        lucas_2n = lucas_n * lucas_n - 2;
    }

    return lucas_2n + 1;   /* a(n) = Lucas(2n) + 1 */
}
