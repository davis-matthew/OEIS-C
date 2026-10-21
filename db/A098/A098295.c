/*
 * A098295  ((3/2)^n)/2^a(n) lies in the half-open interval [1,2).
 * https://oeis.org/A098295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ⌊log₂(3/2)·n⌋ (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* out of offset – defined as 0 */
        return (seq_t)0;

    /* τ = log₂(3/2) ≈ 0.584962500721156.  
       Store  τ·2¹⁴ = 9584.0… and truncate to integer.       */
    static const seq_t SCALE = 9584ULL;   /* floor(τ·2¹⁴) */
    static const uint64_t SHIFT = 14;

    /* Compute ⌊SCALE·n / 2¹⁴⌋.  Both operands fit in the native type. */
    seq_t prod = SCALE * (seq_t)n;           /* S·n  */
    return prod >> SHIFT;                    /* divide by 2¹⁴ */
}
