/*
 * A093851  a(n) = A002283(n-1) + floor(A052268(n)/(1+n)).
 * https://oeis.org/A093851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10^(n-1) - 1 + floor(9 * 10^(n-1) / (n + 1))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute efficiently using only seq_t arithmetic and small integer casts. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset = 1).  Compute p = 10^(n-1). */
    seq_t p = pow10((idx_t)(n - 1));

    /* denom = n + 1 fits into a few bits for the supported seq_t width. */
    seq_t denom = (seq_t)(n + 1);

    /* Compute floor(9 * p / denom) without over‑overflow: */
    seq_t q  = p / denom;          /* integer division  p = q*denom + r  */
    seq_t r  = p % denom;          /* remainder r < denom          */
    seq_t extra = q * 9;           /* 9*floor(p/denom)            */
    seq_t add   = (r * 9) / denom; /* extra part from remainder    */
    extra += add;

    /* Base part: p - 1.  Combine all. */
    return (p - (seq_t)1) + extra;
}
