/*
 * A008979  a(n) = (6n)!/(n!)^6.
 * https://oeis.org/A008979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6n)!/(n!)^6 = Π_{k=2}^{6} binomial(k*n, n)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 2; k <= 6; ++k) {
        result *= binomial(k * n, n);
    }
    return result;
}
