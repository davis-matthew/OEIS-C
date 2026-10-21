/*
 * A226433  The number of permutations of length n in a particular geometric
 *          grid class.
 * https://oeis.org/A226433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*Fib(2n-1) - 2^(n-2) - n + 1 (n > 1), a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_mul.h"    /* seq_mul(seq_t, seq_t) -> seq_t */
#include "../utils/pow2.h"       /* pow2(idx_t) -> seq_t */

/* Fast doubling Fibonacci: computes F(n) and F(n+1) in *fn and *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);

    /* c = a * (2*b - a)   */
    seq_t t = (b << 1) - a;      /* 2*b - a, non‑negative for Fibonacci */
    seq_t c = seq_mul(a, t);

    /* d = a*a + b*b     */
    seq_t d = seq_mul(a, a);
    seq_t bb = seq_mul(b, b);
    d += bb;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        seq_t g = c + d;         /* F(n+1) = c + d */
        *fn  = d;                /* F(n)   = d    */
        *fn1 = g;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) /* offset is 1 */
        return 1;

    /* Fibonacci term F(2n-1) */
    seq_t fib_val;
    seq_t dummy;
    fib_pair(2 * n - 1, &fib_val, &dummy);

    seq_t term = fib_val << 1;           /* 2 * Fib(2n-1) */
    term -= pow2(n - 2);                 /* minus 2^(n-2) */
    term -= (seq_t)n;                    /* minus n */
    term += 1;                           /* plus 1 */

    return term;
}
