/*
 * A027808  a(n) = 55*(n+1)*binomial(n+4,12).
 * https://oeis.org/A027808
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 55*(n+1)*binomial(n+4,12)
 * Range:     u8 none, u16 n <= 10, u32 n <= 20, u64 n <= 105, u128 n <= 1000,
 *            big any n
 * Helpers:   binom_12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_12.h"

seq_t a(idx_t n)
{
    /* Offset is 8; values for n<8 are undefined.
       Some implementations may return 0 for safety. */
    if (n < 8)
        return (seq_t)0;

    seq_t comb   = binom_12(n);
    seq_t factor = (seq_t)55 * (seq_t)(n + 1);
    return factor * comb;
}
