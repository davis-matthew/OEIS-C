/*
 * A155538  Take square root of previous term.
 * https://oeis.org/A155538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^⌊256 / 2^(n-1)⌋ (take square root of previous term)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For values below the offset
       return 0; the behavior is unspecified as per requirements. */
    if (n < 1) return 0;

    /* Compute the exponent e = floor(256 / 2^(n-1)).  When n-1 ≥ 64,
       the shift would be undefined, so we treat e as 0 in that case. */
    uint64_t e = 256ULL;
    idx_t shift = n - 1;
    if (shift < 64) {
        e >>= (unsigned)shift;
    } else {
        e = 0;
    }

    /* Return 2^e.  big_pow handles arbitrary precision seq_t
       correctly, and truncation occurs naturally for small seq_t types. */
    return big_pow((seq_t)2, (seq_t)e);
}
