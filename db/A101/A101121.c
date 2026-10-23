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
 * Helpers:   A101120, pow2_shift
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/A101120.h"
#include "../utils/pow2_shift.h"

/* Public function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 1) return A101120(1);
    seq_t prev = A101120(n - 1);
    seq_t curr = A101120(n);
    return prev ^ curr;
}
