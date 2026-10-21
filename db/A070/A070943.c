/*
 * A070943  Commuting elements: number of ordered pairs g, h in the group
 *          GL(2,Z_n) such that gh = hg.
 * https://oeis.org/A070943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn
 * Formula:   a(n) = A000252(n) * A062354(n) Multiplicative with a(p^e) =
 *            (p^(e+1)-1) * (p-1)^2 * (p+1) * p^(5e-4)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset 1 starts at n = 1 */
    seq_t result = 1;

    uint64_t nn = (uint64_t)n;
    for (uint64_t p = 2; p * p <= nn; ++p)
    {
        if (nn % p == 0)
        {
            int e = 0;
            while (nn % p == 0)
            {
                nn /= p;
                ++e;
            }

            seq_t p_base = p;

            /* p^(e+1) - 1 */
            seq_t pow_pe1 = 1;
            for (int i = 0; i < e + 1; ++i) pow_pe1 *= p_base;
            seq_t part1 = pow_pe1 - 1;

            /* (p-1)^2 */
            seq_t p_minus1 = p_base - 1;
            seq_t p_minus1_sq = p_minus1 * p_minus1;

            /* (p+1) */
            seq_t p_plus1 = p_base + 1;

            /* p^(5e-4) */
            int exp = 5 * e - 4;
            seq_t pow_exp = 1;
            for (int i = 0; i < exp; ++i) pow_exp *= p_base;

            seq_t term = part1 * p_minus1_sq * p_plus1 * pow_exp;
            result *= term;
        }
    }

    if (nn > 1)    /* remaining prime factor (exponent 1) */
    {
        seq_t p_base = nn;

        seq_t pow_pe1 = p_base * p_base;   /* p^(1+1) = p^2 */
        seq_t part1 = pow_pe1 - 1;

        seq_t p_minus1 = p_base - 1;
        seq_t p_minus1_sq = p_minus1 * p_minus1;

        seq_t p_plus1 = p_base + 1;

        seq_t pow_exp = p_base;            /* p^(5*1-4) = p^1 */

        seq_t term = part1 * p_minus1_sq * p_plus1 * pow_exp;
        result *= term;
    }

    return result;
}
