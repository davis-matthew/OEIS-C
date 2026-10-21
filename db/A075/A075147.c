/*
 * A075147  Number of Lyndon words (aperiodic necklaces) with n beads of n
 *          colors.
 * https://oeis.org/A075147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Sum_{d|n} μ(n/d) * n^d
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the number of Lyndon words of length n over an n‑symbol alphabet. */

#include "../utils/mobius.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp)
    {
        if (exp & 1U)
            result *= b;
        /* avoid unnecessary squaring when the exponent is 1 */
        if (exp > 1U)
            b *= b;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Guard against n == 0 (not part of the OEIS offset) */
    if (n == 0)
        return 0;

    seq_t sum_pos = 0;   /* sum of terms with μ = +1 */
    seq_t sum_neg = 0;   /* sum of terms with μ = -1 */

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        idx_t d1 = d;
        idx_t d2 = n / d;

        /* term for divisor d1: μ(n/d1) = μ(d2) */
        seq_t term1 = pow_seq((seq_t)n, d1);
        int mu1 = (int)mobius(d2);   /* μ(d2) */
        if (mu1 > 0)
            sum_pos += term1;
        else if (mu1 < 0)
            sum_neg += term1;

        if (d2 != d1)
        {
            /* term for complementary divisor d2: μ(n/d2) = μ(d1) */
            seq_t term2 = pow_seq((seq_t)n, d2);
            int mu2 = (int)mobius(d1);   /* μ(d1) */
            if (mu2 > 0)
                sum_pos += term2;
            else if (mu2 < 0)
                sum_neg += term2;
        }
    }

    /* The division is exact by number theory */
    seq_t result = (sum_pos - sum_neg) / (seq_t)n;
    return result;
}
