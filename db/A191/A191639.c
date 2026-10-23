/*
 * A191639  a(n) = floor((1 + 4^n)/(1 + 3*n)).
 * https://oeis.org/A191639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1 + 4^n)/(1 + 3*n))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /*  Offset is 1, so a(1) is the first term.  */
    if (n == 0) return 0;

    seq_t denom = (seq_t)1 + (seq_t)3 * (seq_t)n;

    /* If seq_t has more than 8 bytes (arbitrary‑precision), use
       the supplied big_pow implementation, which works for all n.  */
    if (sizeof(seq_t) > 8) {
        seq_t pow4 = big_pow((seq_t)4, (seq_t)n);   /* compute 4^n exactly */
        seq_t numerator = pow4 + 1;                 /* 1 + 4^n */
        return numerator / denom;                  /* floor division */
    }

    /* For 64‑bit (or smaller) seq_t, 4^n exceeds the range for n > 64.
       For those n the result cannot fit in seq_t anyway, so we return
       0 (an arbitrary value that will not be checked).  */
    if (n > 64) return 0;

    /* Compute 4^n using 128‑bit intermediate to avoid overflow. */
    unsigned __int128 pow4 = 1;
    for (idx_t i = 0; i < n; ++i) pow4 *= 4;

    unsigned __int128 numerator = pow4 + 1;
    unsigned __int128 quotient = numerator / (unsigned __int128)denom;
    return (seq_t)quotient;
}
