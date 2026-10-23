/*
 * A023975  First bit in fractional part of binary expansion of 8th root of n.
 * https://oeis.org/A023975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the fractional part of n^(1/8) ≥ 0.5, otherwise 0.
 *            It is equivalent to a(n) = floor(2·{ n^(1/8) }). The value is
 *            computed by comparing 256·n with (2·⌊n^(1/8)⌋+1)^8
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   int_root_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_root_8.h"

static inline __int128 pow128_uint64(uint64_t base, unsigned exp)
{
    __int128 result = 1;
    for (unsigned i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; behaviour for n = 0 is irrelevant. */
    uint64_t m = (uint64_t)n;           /* convert to native integer for arithmetic */
    uint64_t a0 = int_root_8(m);        /* floor of nth eighth root */
    __int128 lhs = (__int128)m << 8;    /* 256 * n */
    __int128 rhs = pow128_uint64(2*a0 + 1, 8);  /* (2*a0+1)^8 */

    return (seq_t)((lhs >= rhs) ? 1 : 0);
}
