/*
 * A115132  Partial sums of A064059.
 * https://oeis.org/A115132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=0}^{5} c_i * binom(n-i+7, 7)
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where c = {132, -495, 770, -616, 252, -42} */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    const long long coeff[6] = {132, -495, 770, -616, 252, -42};
    seq_t result = 0;

    for (int i = 0; i < 6; ++i)
    {
        if ((idx_t)i > n)
            continue;
        idx_t k = n - (idx_t)i + 7;
        seq_t bin = binomial(k, 7);
        if (coeff[i] >= 0)
            result += (seq_t)coeff[i] * bin;
        else
            result -= (seq_t)(-coeff[i]) * bin;
    }
    return result;
}
