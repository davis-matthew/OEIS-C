/*
 * A171823  a(n) = base-2 concatenation XYZ, where X = number of bits in
 *          binary expansion of n, Y = number of 0's, Z = number of 1's.
 * https://oeis.org/A171823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary concatenation of the binary representations of X =
 *            number of bits in n, Y = number of 0's in the binary expansion
 *            of n, Z = number of 1's in the binary expansion of n interpreted
 *            as decimal digits (0’s and 1’s)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   append_bits, floor_log2_uint64, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation follows the definition using only
   integer arithmetic; the helper functions floor_log2_uint64
   and popcount_u64 are taken from the pre‑provided utilities. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"            /* unused but placeholder if needed */
#include "../utils/pow_seq.h"          /* unused but placeholder if needed */
#include "../utils/append_bits.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset 1 – this value is irrelevant */

    /* X : number of bits of n */
    size_t bitsX = floor_log2_uint64((idx_t)n) + 1;
    uint64_t X = bitsX;

    /* Y and Z : counts of 0's and 1's in n */
    size_t ones = 0;
    idx_t tmp = n;
    while (tmp) {
        ++ones;
        tmp &= (tmp - 1);            /* pop the lowest set bit */
    }
    size_t zeros = bitsX - ones;
    uint64_t Y = zeros;              /* zeros may be 0 */
    uint64_t Z = ones;               /* ones ≥ 1 for n ≥ 1 */

    /* bit lengths of X, Y, Z in binary */
    size_t bitsY = (Y == 0) ? 1 : floor_log2_uint64((idx_t)Y) + 1;
    size_t bitsZ = floor_log2_uint64((idx_t)Z) + 1;

    seq_t result = 0;
    result = append_bits(result, X, bitsX);
    result = append_bits(result, Y, bitsY);
    result = append_bits(result, Z, bitsZ);

    return result;
}
