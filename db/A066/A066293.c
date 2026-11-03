/*
 * A066293  a(n) = A000203(n)^2 - A001157(n) = sigma(n)^2 - sigma_2(n).
 * https://oeis.org/A066293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n)^2 - sigma_2(n) (A066293)
 * Range:     u8 n <= 11, u16 n <= 119, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9482 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

static void sigma1_sigma2(uint64_t n, unsigned __int128 *sigma1, unsigned __int128 *sigma2)
{
    unsigned __int128 s1 = 1, s2 = 1;
    uint64_t remaining = n;
    for (uint64_t p = 2; p * p <= remaining; ++p)
    {
        if (remaining % p == 0)
        {
            int exp = 0;
            while (remaining % p == 0)
            {
                remaining /= p;
                ++exp;
            }

            /* compute (p^{exp+1} - 1) / (p - 1)   for sigma1 */
            unsigned __int128 pow_p = 1;
            for (int i = 0; i < exp + 1; ++i) pow_p *= p;
            unsigned __int128 part1 = (pow_p - 1) / (p - 1);

            /* compute (p^{2(exp+1)} - 1) / (p^2 - 1)   for sigma2 */
            unsigned __int128 pow_p2 = pow_p * pow_p;   /* p^{2(exp+1)} */
            unsigned __int128 part2 = (pow_p2 - 1) / ((p * p) - 1);

            s1 *= part1;
            s2 *= part2;
        }
    }
    if (remaining > 1)   /* remaining prime factor with exponent 1 */
    {
        uint64_t p = remaining;
        unsigned __int128 pow_p = (unsigned __int128)p * p;   /* p^{1+1} = p^2 */
        unsigned __int128 part1 = (pow_p - 1) / (p - 1);

        unsigned __int128 pow_p2 = pow_p * pow_p;   /* p^{4} = (p^2)^2 */
        unsigned __int128 part2 = (pow_p2 - 1) / ((p * p) - 1);

        s1 *= part1;
        s2 *= part2;
    }
    *sigma1 = s1;
    *sigma2 = s2;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* seq_t may be unsigned; values for n<1 are irrelevant */

    unsigned __int128 s1, s2;
    sigma1_sigma2((uint64_t)n, &s1, &s2);

    unsigned __int128 result = (s1 * s1) - s2;
    return (seq_t)result;
}
