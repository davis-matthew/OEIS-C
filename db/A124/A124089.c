/*
 * A124089  a(n) = binomial(n,6)-1.
 * https://oeis.org/A124089
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = binomial(n,6)-1
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 6 the sequence is undefined; returning 0 is acceptable for these cases. */
    if (n < 6) {
        return (seq_t)0;
    }
    return binomial(n, 6) - (seq_t)1;
}
