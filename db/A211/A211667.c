/*
 * A211667  Number of iterations sqrt(sqrt(sqrt(...(n)...))) such that the
 *          result is < 2.
 * https://oeis.org/A211667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0; for n≥2, a(n)=1+⌊log₂(⌊log₂(n)⌋)⌋
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    unsigned u1 = floor_log2_uint64(n);
    unsigned u2 = floor_log2_uint64(u1);
    return (seq_t)(1 + u2);
}
