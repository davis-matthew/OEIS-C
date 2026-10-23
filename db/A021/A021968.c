/*
 * A021968  Decimal expansion of 1/964.
 * https://oeis.org/A021968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the n‑th digit after the decimal point in the expansion
 *            of 1/964. The digit is obtained by the usual long‑division rule:
 *            rem₀ = 1 dᵢ = ⌊(remᵢ * 10) / 964⌋ remᵢ₊₁ = (remᵢ * 10) mod 964
 *            For index n the function returns dₙ
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Remainder never exceeds 963, so a narrow integer type is safe. */
    uint64_t rem = 1;
    seq_t digit = 0;

    /* Compute digits from 0 up to n inclusive. */
    for (idx_t i = 0; i <= n; ++i) {
        digit = (seq_t)(rem * 10 / 964);
        rem = (rem * 10) % 964;
    }
    return digit;
}
