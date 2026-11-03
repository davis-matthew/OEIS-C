/*
 * A098232  Largest power of 2 <= 3^n.
 * https://oeis.org/A098232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^floor(n*log₂(3)) (largest power of 2 ≤ 3ⁿ)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 911 (2026-11-01)
 * Helpers:   bit_length, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute 3ⁿ */
    seq_t p = pow3(n);

    /* Find floor(log₂(3ⁿ)) as bit length minus one */
    unsigned bl = bit_length(p);
    idx_t   e  = (idx_t)(bl - 1);

    /* Return 2ᵉ, the desired power of two */
    return pow2(e);
}
