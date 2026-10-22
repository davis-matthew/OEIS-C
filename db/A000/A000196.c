/*
 * A000196  Integer part of square root of n. Or, number of positive squares
 *          <= n. Or, n appears 2n+1 times.
 * https://oeis.org/A000196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = floor(sqrt(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a196_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a196_isqrt.h"

seq_t a(idx_t n)
{
    return (seq_t)a196_isqrt((uint64_t)n);
}
