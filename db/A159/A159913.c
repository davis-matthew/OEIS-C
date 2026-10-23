/*
 * A159913  a(n) = 2^(A000120(n) + 1) - 1, where A000120(n) = number of
 *          nonzero bits in n.
 * https://oeis.org/A159913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(A000120(n)+1) - 1, where A000120(n) = popcount(n)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Count set bits in n (A000120).  n is uint64_t (idx_t).  */
    uint64_t bitcnt = __builtin_popcountll((uint64_t)n);

    /* Compute 2^(bitcnt+1) and then subtract 1.  */
    seq_t result = pow2((idx_t)(bitcnt + 1)) - 1;
    return result;
}
