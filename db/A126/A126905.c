/*
 * A126905  Coordination sequence for 20-dimensional cyclotomic lattice
 *          Z[zeta_25].
 * https://oeis.org/A126905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊n/10⌋} C(5,k) * C(n-10k+19, 19) (coefficient of
 *            x^n in (1+x^10)^5/(1-x)^20)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    const unsigned char combo5[6] = {1,5,10,10,5,1};
    seq_t result = 0;

    /* Loop over k such that 10k ≤ n (at most 6 terms) */
    for (idx_t k = 0; k <= 5 && k * 10 <= n; ++k)
    {
        idx_t m = n - 10 * k;          /* m ≥ 0 */
        seq_t coeff5 = (seq_t)combo5[k];

        /* Compute C(m + 19, 19) multiplicatively */
        seq_t bin = 1;
        for (idx_t i = 1; i <= 19; ++i)
        {
            /* bin = bin * (m + i) / i   (exact division) */
            bin = bin * ((seq_t)m + (seq_t)i) / (seq_t)i;
        }

        result += coeff5 * bin;
    }

    return result;
}
