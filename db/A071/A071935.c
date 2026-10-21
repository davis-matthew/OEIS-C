/*
 * A071935  K(n,n+1) where K(x,y) is the Kronecker symbol (x/y).
 * https://oeis.org/A071935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = K(n, n+1) where K is the Kronecker symbol (·/·)
 * Range:     i8 n <= 107, i16 n <= 107, i32 n <= 107, i64 n <= 107,
 *            i128 n <= 107, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* Compute the Kronecker symbol (n | n+1) */

    /* Handle the trivial case n = 0 (should not occur) */
    if (n == 0)
        return 0;

    /* Separate the denominator into its 2-adic part and odd part */
    idx_t denom = n + 1;          /* n+1 is >0 */
    idx_t k = 0;                  /* exponent of 2 in denom */
    while ((denom & 1) == 0) {
        denom >>= 1;
        ++k;
    }

    /* Compute the Jacobi part for the odd part of the denominator */
    seq_t r = 1;                  /* default value if denom becomes 1 */
    if (denom != 1) {
        r = (seq_t)jacobi_symbol((uint64_t)n, (uint64_t)denom);
    }

    /* Compute (n/2)^k.
       Since n is odd when k>0, (n/2) is either +1 or -1. */
    uint8_t mod8 = (uint8_t)(n & 7ull);
    int base = (mod8 == 1 || mod8 == 7) ? 1 : -1;   /* (n/2) value */

    int sign = (k % 2 == 0) ? 1 : base;              /* (n/2)^k */

    return r * (seq_t)sign;
}
