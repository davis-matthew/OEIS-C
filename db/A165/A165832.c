/*
 * A165832  Totally multiplicative sequence with a(p) = 11.
 * https://oeis.org/A165832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 11^bigomega(n)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* compute the number of prime factors of n (with multiplicity) */
    seq_t omega = bigomega(n);
    /* exponent is tiny (≤log₂n), cast to idx_t for the loop counter */
    idx_t exp = (idx_t)omega;

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)11;

    for (idx_t i = 0; i < exp; ++i)
        result = result * base;

    return result;
}
