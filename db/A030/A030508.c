/*
 * A030508  Graham-Sloane-type lower bound on the size of a ternary (n,3,8)
 *          constant-weight code.
 * https://oeis.org/A030508
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling( binomial(n,8) * 256 / (2*n + 1) )
 * Range:     u8 none, u16 n <= 11, u32 n <= 33, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function binom(n,k) from utils/binom.h */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 8) return 0;        /* undefined for n < 8, return 0 for safety */

    /* binomial coefficient C(n,8) */
    seq_t c = binom(n, (idx_t)8);

    /* numerator: C(n,8) * 256 */
    seq_t num = c * (seq_t)256;

    /* denominator: 2*n + 1 */
    seq_t den = (seq_t)2 * (seq_t)n + (seq_t)1;

    /* ceil division: (num + den - 1) / den */
    return (num + den - (seq_t)1) / den;
}
