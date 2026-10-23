/*
 * A186418  a(n) = binomial(2*n,n)^4/(n + 1)^2.
 * https://oeis.org/A186418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*n,n)^4/(n + 1)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t b   = binom(2 * n, n);                 /* C(2n,n)          */
    seq_t div = (seq_t)(n + 1);                  /* n+1              */
    seq_t c   = b / div;                         /* Catalan number   */

    seq_t c2  = c * c;                           /* c^2              */
    seq_t c4  = c2 * c2;                         /* c^4              */
    seq_t d2  = div * div;                       /* (n+1)^2          */

    return c4 * d2;                              /* a(n)              */
}
