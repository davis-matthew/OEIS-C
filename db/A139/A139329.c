/*
 * A139329  a(n) = (factorial of the number of 0's in the binary expansion of
 *          n).
 * https://oeis.org/A139329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = factorial(number of 0's in the binary expansion of n)
 * Range:     u8 n <= 63, u16 n <= 511, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   factorial, floor_log2_uint64, popcount_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/popcount_u64.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Number of bits needed to represent n.
       For n == 0, we consider the single bit '0', so bits = 1. */
    idx_t bits = (n == 0) ? 1ULL : (floor_log2_uint64(n) + 1ULL);

    /* Count of 1-bits and consequently the count of 0-bits. */
    idx_t ones  = popcount_u64(n);
    idx_t zeros = bits - ones;

    /* Return factorial of the number of zeros. */
    return factorial(zeros);
}
