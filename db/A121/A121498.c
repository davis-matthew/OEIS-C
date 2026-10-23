/*
 * A121498  Numerators of partial alternating sums of Catalan numbers scaled
 *          by powers of 1/(29^2) = 1/841.
 * https://oeis.org/A121498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the partial alternating sum * r(n) =
 *            Σ_{k=0..n} (-1)^k · Catalan(k) / 841^k , * reduced to lowest
 *            terms. The denominator of r(n) is 841^n * (i.e. 29^(2n)); all
 *            common powers of 29 are removed from the * numerator before it
 *            is returned
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Compute 841^n as the common denominator (as an integer). */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow *= (seq_t)841;          /* pow = 841ⁿ */

    /* Sum the terms with the common denominator 841ⁿ. */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = catalan(k) * pow;   /* (-1)^k * C(k) * 841ⁿ⁻ᵏ  */
        if ((k & 1u) == 0)
            sum += term;
        else
            sum -= term;

        if (k < n)
            pow /= (seq_t)841;          /* next power: 841ⁿ⁻ᵏ⁻¹  */
    }

    /* Remove all common factors of 29 from the numerator.  Since the
     * denominator contains exactly 29^(2n), any 29‑factor in the
     * numerator can be cancelled. */
    while ((sum % (seq_t)29) == 0)
        sum /= (seq_t)29;

    return sum;
}
