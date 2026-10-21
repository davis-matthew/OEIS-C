/*
 * A092441  Sequence arising from enumeration of domino tilings of Aztec
 *          Pillow-like regions.
 * https://oeis.org/A092441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2n+3) - 2^(n+2) - 2*(n+2)*(2^(n+1)-1) + (n+1)^2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_uint64.h"

seq_t a(idx_t n)
{
    /* 2^(2n+3) */
    seq_t t1 = pow2_uint64( 2ULL * (uint64_t)n + 3ULL );

    /* 2^(n+2) */
    seq_t t2 = pow2_uint64( (uint64_t)n + 2ULL );

    /* 2^(n+1) */
    seq_t t3pow = pow2_uint64( (uint64_t)n + 1ULL );

    seq_t term1 = t1;
    seq_t term2 = t2;
    seq_t term3 = (seq_t)2 * ((seq_t)(n + 2ULL)) * (t3pow - 1);
    seq_t term4 = ((seq_t)(n + 1ULL)) * ((seq_t)(n + 1ULL));

    return term1 - term2 - term3 + term4;
}
