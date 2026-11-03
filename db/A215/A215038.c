/*
 * A215038  Partial sums of A066259: a(n) = Sum_{k=0..n} F(k+1)^2*F(k), n>=0,
 *          with the Fibonacci numbers F=A000045.
 * https://oeis.org/A215038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(n+2)*F(n+1)^2 - (-1)^n*F(n) - 1)/2 where F(0)=0,
 *            F(1)=1 are Fibonacci numbers
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*         fn1 – F(n+1) (seq_t *)                          */
static void fib_pair_fast(idx_t n, seq_t *fn, seq_t *fn1)
{
    seq_t a = 0;   /* F(0)   */
    seq_t b = 1;   /* F(1)   */

    /* Process bits of n from most significant to least   */
    for (int i = 63; i >= 0; --i) {
        /* Compute c = F(2k)  = a*(2*b - a)   */
        /* Compute d = F(2k+1)= a*a + b*b       */
        seq_t t1 = a * (((seq_t)2 * b) - a);      /* 2*b - a is non‑negative (a <= b) */
        seq_t t2 = a * a + b * b;

        if (((n >> i) & 1ULL) == 0) {
            /* k -> 2k   */
            a = t1;
            b = t2;
        } else {
            /* k -> 2k+1 */
            a = t2;
            b = t1 + t2;
        }
    }
    *fn  = a;
    *fn1 = b;
}

/* Fast‑doubling algorithm to compute F(n) and F(n+1)   */
/* Input:  n – index (idx_t)                               */
/* Output: fn  – F(n)   (seq_t *)                          */

seq_t a(idx_t n)
{
    seq_t fn, fn1;

    fib_pair_fast(n, &fn, &fn1);        /* fn  = F(n),   fn1 = F(n+1)   */
    seq_t fn2 = fn1 + fn;               /* F(n+2)         */

    seq_t fn1_sq = fn1 * fn1;           /* F(n+1)^2       */
    seq_t term1  = fn2 * fn1_sq;        /* F(n+2)*F(n+1)^2*/

    seq_t numerator;
    if ((n & 1ULL) == 0) {              /* n even: subtract F(n)   */
        numerator = term1 - fn - 1;
    } else {                            /* n odd: add F(n)          */
        numerator = term1 + fn - 1;
    }

    return numerator / 2;
}
