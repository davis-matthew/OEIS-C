/*
 * A062261  Fourth (unsigned) column sequence of triangle A062140 (generalized
 *          a=4 Laguerre).
 * https://oeis.org/A062261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)! * binomial(n+7,7) / 3! (3! = 6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, factorial_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t a, idx_t b) */
#include "../utils/factorial_seq.h"

seq_t a(idx_t n)
{
    /* (n+3)!  */
    seq_t fact = factorial_seq(n + 3);

    /* Divide first to avoid overflow: fact is divisible by 6 for all n >= 0. */
    seq_t fact_div = fact / 6;           /* (n+3)! / 3! */

    /* C(n+7, 7)  */
    seq_t bin = binomial(n + 7, 7);

    return fact_div * bin;
}
