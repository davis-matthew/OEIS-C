/*
 * A019554  Smallest number whose square is divisible by n.
 * https://oeis.org/A019554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, nice
 * Formula:   a(n) = product of p^ceil(e/2) over all prime powers p^e dividing
 *            n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1                                              */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;         /* n is at least 1 */
    seq_t result = 1;

    /* trial‑division factorisation */
    for (uint64_t p = 2; p*p <= m; ++p)
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            idx_t exp = (e + 1) / 2;    /* ceil(e/2) */
            seq_t term = 1;
            for (idx_t i = 0; i < exp; ++i)
                term = term * (seq_t)p;
            result = result * term;
        }
    }

    /* remainder prime factor > sqrt(original n) */
    if (m > 1)
        result = result * (seq_t)m;   /* exponent 1 → ceil(1/2)=1 */

    return result;
}
