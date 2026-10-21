/*
 * A053310  a(n) = (n+3)*binomial(n+8, 8)/3.
 * https://oeis.org/A053310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+3)*binomial(n+8, 8)/3 (OEIS A053310, offset 0)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 38, u64 n <= 503, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute C(n+8, 8).  The binomial routine works for any n>=0. */
    seq_t bin = binomial(n + 8, (idx_t)8);

    /* To avoid overflow when seq_t is 64 bit, divide by 3
       on the factor that is guaranteed to be divisible by 3. */
    if ((n % 3) == 0) {
        /* (n+3) is a multiple of 3 here.  Divide it first. */
        seq_t t = ((seq_t)(n + 3)) / 3;
        return t * bin;
    } else {
        /* Otherwise C(n+8,8) is a multiple of 3.  Divide it. */
        seq_t t = bin / 3;
        return t * ((seq_t)(n + 3));
    }
}
