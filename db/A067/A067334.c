/*
 * A067334  Convolution of Fibonacci F(n+1), n>=0, with F(n+6), n>=0.
 * https://oeis.org/A067334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((29*n+40)*F(n+1)+18*(n+1)*F(n))/5, where F(m) is the
 *            m‑th Fibonacci number (F(0)=0,F(1)=1). This uses fast doubling
 *            to compute F(n) and F(n+1) in O(log n) time
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   // for NULL (only needed for static cast compatibility)

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;                      /* (F(k), F(k+1)) for k = n/2 */
    fib_pair(n >> 1, &a, &b);

    seq_t t = (seq_t)2 * b - a;       /* t = 2*F(k+1) - F(k)   */
    seq_t c = a * t;                 /* c = F(2k)             */
    seq_t d = a * a + b * b;         /* d = F(2k+1)           */

    if ((n & 1) == 0) {              /* even n */
        *fn  = c;
        *fn1 = d;
    } else {                         /* odd  n */
        *fn  = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);          /* fn  = F(n),   fn1 = F(n+1) */

    seq_t part1 = ((seq_t)29 * (seq_t)n) + (seq_t)40;      /* (29*n+40) */
    seq_t term1 = part1 * fn1;                               /* *F(n+1) */
    seq_t term2 = ((seq_t)18 * ((seq_t)n + 1)) * fn;         /* 18*(n+1)*F(n) */

    seq_t num = term1 + term2;                              /* numerator */
    return num / (seq_t)5;                                  /* divide by 5 */
}
