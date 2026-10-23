/*
 * A166670  Totally multiplicative sequence with a(p) = 5p-2 for prime p.
 * https://oeis.org/A166670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product_{p^e || n} (5*p - 2)^e
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t, seq_t types (defined by the build) */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1 */

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* Trial division: factor out powers of each prime. */
    for (idx_t d = 2; d <= m / d; d += (d == 2 ? 1 : 2))
    {
        if (m % d == 0)
        {
            idx_t e = 0;
            while (m % d == 0)
            {
                m /= d;
                ++e;
            }
            seq_t factor = (seq_t)((__int128)5 * d - 2);
            seq_t pow_f = (seq_t)1;
            for (idx_t i = 0; i < e; ++i)
                pow_f *= factor;
            result *= pow_f;
        }
    }

    /* Remaining prime factor, if any. */
    if (m > 1)
    {
        seq_t factor = (seq_t)((__int128)5 * m - 2);
        result *= factor;
    }

    return result;
}
