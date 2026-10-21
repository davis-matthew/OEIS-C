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
 * Helpers:   fib_pair_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_pair_fast.h"

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
