/*
 * A126920  Coordination sequence for 18-dimensional cyclotomic lattice
 *          Z[zeta_27].
 * https://oeis.org/A126920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..min(9, ⌊n/3⌋)} C(9, i) * C(17 + n-3i, n-3i)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses binomial coefficients computed multiplicatively to avoid
   dynamic allocation and is valid under C11 and C++17. */

#include <stdint.h>

static const seq_t C9[10] = {
    (seq_t)1, (seq_t)9, (seq_t)36, (seq_t)84,
    (seq_t)126, (seq_t)126, (seq_t)84, (seq_t)36,
    (seq_t)9, (seq_t)1
};

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* i only runs up to 9, and also to ⌊n/3⌋ */
    idx_t i_max = n / 3;
    if (i_max > 9) i_max = 9;

    for (idx_t i = 0; i <= i_max; ++i)
    {
        idx_t j = n - 3 * i;               /* j = n - 3i ≥ 0 */

        /* Compute C(17 + j, j) = C(17 + j, 17) by multiplicative formula */
        seq_t bin = (seq_t)1;
        for (idx_t k = 1; k <= j; ++k)
        {
            bin = bin * (seq_t)(17 + k);
            bin = bin / (seq_t)k;
        }

        /* Accumulate term */
        result += C9[i] * bin;
    }

    return result;
}
