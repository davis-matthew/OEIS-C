/*
 * A054542  A Catalan-like sequence.
 * https://oeis.org/A054542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( binom(2*n, n)/(n+2) )
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t twon = n + n;                 /* 2*n as an index */
    seq_t bin  = binom(twon, n);        /* exact binomial C(2n, n) */
    seq_t den  = (seq_t)n + 2;          /* denominator n+2 as seq_t */
    return (bin + den - 1) / den;       /* ceil division: (bin + den -1)/den */
}
