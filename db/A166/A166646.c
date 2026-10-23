/*
 * A166646  Totally multiplicative sequence with a(p) = 6*(p+1) for prime p.
 * https://oeis.org/A166646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} (6*(p+1))^e (multiplicative, a(p)=6*(p+1))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* trial division factorisation up to sqrt(m) */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            /* base = 6*(p+1) */
            seq_t base = (seq_t)(6ULL * (p + 1ULL));
            for (int i = 0; i < exp; ++i)
                result *= base;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1)
    {
        seq_t base = (seq_t)(6ULL * (m + 1ULL));
        result *= base;
    }

    return result;
}
