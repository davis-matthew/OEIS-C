/*
 * A130015  Number of isomers for different number of substitutions of
 *          hydrogen atoms by deuterium in the boat form of cyclohexane.
 * https://oeis.org/A130015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   a(n) = ( C(12,n) + 2*δ_even(n)*C(6, n/2) + Σ_{k=0..4} C(4,k) *
 *            C(4, n-2k) ) / 4
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where δ_even(n) = 1 if n is even, otherwise 0.  This implements the Burnside formula
   for the D₂ symmetry group of the cyclohexane boat. */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* The boat has only 12 hydrogen atoms, so for n>12 there are no isomers. */
    if (n > 12) return 0;

    seq_t sum = binom((idx_t)12, n);          /* identity element */

    /* Three C₂ rotations with cycle structure (2⁶). Two of them contribute only
       for even n. */
    if ((n & 1) == 0) {
        sum += (seq_t)2 * binom((idx_t)6, n / 2);
    }

    /* One C₂ rotation with cycle structure (1⁴ 2⁴). */
    for (int k = 0; k <= 4; ++k) {
        int k1 = (int)n - 2 * k;              /* remaining weight from 1‑cycles */
        if (k1 < 0 || k1 > 4) continue;
        sum += binom((idx_t)4, (idx_t)k) * binom((idx_t)4, (idx_t)k1);
    }

    return sum / (seq_t)4;                   /* divide by |G| = 4 */
}
