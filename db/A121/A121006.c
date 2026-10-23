/*
 * A121006  Numerators of partial alternating sums of Catalan numbers scaled
 *          by powers of 1/5.
 * https://oeis.org/A121006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of r(n) = Σ_{k=0..n} (-1)^k * Catalan(k) / 5^k,
 *            reduced to lowest terms
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* pow_factor = 5^n.  We build it iteratively; for very large n
       the arbitrary‑precision type can hold the value.                     */
    seq_t pow_factor = 1;
    for (idx_t i = 0; i < n; ++i)
        pow_factor *= 5;

    seq_t pos = 0;            /* sum of terms with even k (positive) */
    seq_t neg = 0;            /* sum of terms with odd  k (negative) */
    seq_t C   = 1;            /* Catalan(k), starts with C(0)=1 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = C * pow_factor;
        if (k & 1)
            neg += term;
        else
            pos += term;

        if (k == n)  /* last iteration, nothing to update */
            break;

        /* Update Catalan number to C(k+1) efficiently:
           C_{k+1} = C_k * 2*(2*(k+1)-1) / (k+2)   (exact division) */
        seq_t mult = (seq_t)(2 * (2 * (k + 1) - 1));
        C = (C * mult) / (k + 2);

        /* Prepare power factor for the next term: 5^{n-(k+1)}   */
        pow_factor /= 5;
    }

    /* Difference of the two sums gives the numerator of r(n) before
       reduction.  It must be positive for all n, so pos >= neg.     */
    seq_t sum = pos - neg;

    /* Reduce by powers of 5: divide out all factors of 5. */
    while (sum % 5 == 0)
        sum /= 5;

    return sum;
}
