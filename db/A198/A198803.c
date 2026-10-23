/*
 * A198803  Number of closed paths of length 17n whose steps are 17th roots of
 *          unity.
 * https://oeis.org/A198803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (17*n)!/(n!)^17
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t total = 17 * n;
    seq_t result = (seq_t)1;

    for (idx_t i = 0; i < 17; ++i) {
        result *= binomial(total, n);
        total -= n;
    }

    return result;
}
