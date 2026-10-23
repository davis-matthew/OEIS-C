/*
 * A010947  Binomial coefficient C(31,n).
 * https://oeis.org/A010947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(31,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 14, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n > 31) return 0;
    return binomial((idx_t)31, n);
}
