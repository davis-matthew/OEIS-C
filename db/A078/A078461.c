/*
 * A078461  a(n) = 0 if n is divisible by the square of odd prime, a(n) = 1 if
 *          n is an odd squarefree number, a(n) = 2 otherwise.
 * https://oeis.org/A078461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = 0 if n has a square of an odd prime factor, a(n) = 1 if n
 *            is odd and square‑free, a(n) = 2 otherwise
 * Range:     u8 n <= 9643, u16 n <= 9643, u32 n <= 9643, u64 n <= 9643,
 *            u128 n <= 9643, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n >= 1 for all calls by the test harness */
    idx_t m = n;
    /* will be set to true if an odd prime square factor is found */
    bool bad_square = false;

    /* trial division up to sqrt(m) */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            unsigned e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            if ((p & 1) && e >= 2)   /* odd prime with exponent ≥ 2 */
            {
                bad_square = true;
                break;
            }
        }
    }

    if (bad_square)
        return (seq_t)0;

    /* Any remaining factor > 1 is a prime with exponent 1 */
    /* (it cannot be a square because we finished dividing). */

    /* Final classification */
    return (n & 1) ? (seq_t)1 : (seq_t)2;
}
