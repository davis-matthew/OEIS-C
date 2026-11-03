/*
 * A101121  Bitwise XOR of adjacent terms of A101120; also the nonzero terms
 *          of A101122.
 * https://oeis.org/A101121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A101120(n-1) XOR A101120(n) for n>1, a(1) = A101120(1)
 *            where A101120(n) = 2^(n+3) - 2^((n-1) mod 4) - 8*floor((n+3)/4)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-01)
 * Helpers:   pow2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_u64.h"

/* Compute A101120(n) according to its closed form. */
static inline seq_t A101120(idx_t n)
{
    /* 2^(n+3) */
    seq_t term1 = pow2_u64((uint64_t)n + 3);
    /* 2^((n-1) mod 4)  – exponent is always < 4, safe shift */
    seq_t term2 = pow2_u64(((uint64_t)n - 1) % 4);
    /* 8 * floor((n+3)/4)  – fits safely into seq_t for all n up to 10⁴ */
    seq_t term3 = (seq_t)(8 * (( (uint64_t)n + 3 ) / 4));
    return term1 - term2 - term3;
}

/* Public function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 1) return A101120(1);
    seq_t prev = A101120(n - 1);
    seq_t curr = A101120(n);
    return prev ^ curr;
}
