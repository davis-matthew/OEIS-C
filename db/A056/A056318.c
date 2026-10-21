/*
 * A056318  Number of primitive (aperiodic) reversible strings with n beads
 *          using exactly two different colors.
 * https://oeis.org/A056318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * ((2^{n/d} + 2^{⌈(n/d)/2⌉}) / 2), with a(1)
 *            = 0. μ is the Möbius function. The inner term counts all
 *            reversible binary strings of length n/d (palindromes are handled
 *            by the ceil exponent)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   mu_of, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t sum = (seq_t)0;

    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d)
            continue;

        int mu = mu_of(d);
        if (mu != 0)
        {
            idx_t t = n / d;
            seq_t val = (pow_uint(2ULL, t) +
                         pow_uint(2ULL, (t + 1) / 2)) / 2;
            if (mu > 0)
                sum += val;
            else
                sum -= val;
        }

        idx_t d2 = n / d;
        if (d2 != d)
        {
            mu = mu_of(d2);
            if (mu != 0)
            {
                idx_t t = n / d2; /* t = d */
                seq_t val = (pow_uint(2ULL, t) +
                             pow_uint(2ULL, (t + 1) / 2)) / 2;
                if (mu > 0)
                    sum += val;
                else
                    sum -= val;
            }
        }
    }

    return sum;
}
