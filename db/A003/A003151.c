/*
 * A003151  Beatty sequence for 1+sqrt(2); a(n) = floor(n*(1+sqrt(2))).
 * https://oeis.org/A003151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(1+sqrt(2))) = n + floor(sqrt(2*n^2)), computed
 *            with an exact integer square root
 * Range:     u8 n <= 106, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   a003151_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003151_isqrt128.h"

seq_t a(idx_t n)
{
    unsigned __int128 nn = (unsigned __int128)n;
    uint64_t r = a003151_isqrt128(2 * nn * nn);
    seq_t result = (seq_t)n;
    result += (seq_t)r;
    return result;
}
