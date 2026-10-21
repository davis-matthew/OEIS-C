/*
 * A071974  Numerator of rational number i/j such that Sagher map sends i/j to
 *          n.
 * https://oeis.org/A071974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy, nice, mult
 * Formula:   a(n) = Π p_i^f(e_i) where n = Π p_i^e_i and f(e) = e/2 if e is
 *            even, f(e) = 0 if e is odd. This is equivalent to a(n) =
 *            sqrt(A350388(n)), the square root of the largest unitary divisor
 *            of n that is a square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = n;

    /* factorization by trial division (fast enough for n <= 10000 and
       even for larger n within the 2‑second budget). */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            unsigned exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            if ((exp & 1) == 0)        /* exponent is even */
            {
                unsigned half = exp >> 1;
                seq_t powp = 1;
                for (unsigned i = 0; i < half; ++i)
                    powp *= (seq_t)p;
                result *= powp;
            }
        }
    }
    /* Remaining prime factor (if any) has exponent 1, which is odd,
       so it contributes nothing to a(n). */
    return result;
}
