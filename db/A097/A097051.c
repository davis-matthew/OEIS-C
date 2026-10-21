/*
 * A097051  a(n) = floor(n/a(floor(n/2))); a(1) = 1.
 * https://oeis.org/A097051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097051:
   a(1) = 1
   a(n) = floor( n / a(floor(n/2)) )
   Equivalent closed form:
       if ⌊log₂ n⌋ = 2k  → a(n) = 2ᵏ
       if ⌊log₂ n⌋ = 2k+1→ a(n) = ⌊n / 2ᵏ⌋
   (n ≥ 1)
*/
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n <= 1)
        return (seq_t)1;

    /* Compute floor(log₂ n) using the helper from the utils crate. */
    unsigned log2 = floor_log2_uint64(n);   /* log2 in [0, 63] for idx_t */
    unsigned half_log = log2 >> 1;          /* k = ⌊log₂ n⌋ / 2 */

    if ((log2 & 1u) == 0)                   /* even log₂ → a(n) = 2ᵏ */
    {
        return ((seq_t)1) << half_log;
    }
    else                                    /* odd log₂ → a(n) = ⌊n / 2ᵏ⌋ */
    {
        return (seq_t)(n >> half_log);
    }
}
