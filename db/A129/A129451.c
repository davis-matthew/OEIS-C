/*
 * A129451  Expansion of f(-x, -x^3) f(-x, x^2) in powers of x where f(, ) is
 *          Ramanujan's general theta function.
 * https://oeis.org/A129451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(6*n + 1), where b is multiplicative defined as * b(2^e)
 *            = 0^e (so b(2^e) = 0 for e > 0, b(1) = 1) * b(3^e) = 0^e
 *            (analogous) * for primes p ≡ 5 or 11 (mod 12): * b(p^e) = (1 +
 *            (-1)^e) / 2 // 1 if e even, else 0 * for primes p ≡ 1 (mod 12):
 *            * b(p^e) = e + 1 * for primes p ≡ 7 (mod 12): *...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to the argument m = 6*n + 1 */
    uint64_t m = 6ULL * (uint64_t)n + 1ULL;

    /* Result of the multiplicative function b(m) */
    seq_t result = 1;

    /* Trial division for prime powers */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint32_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }

            seq_t term;
            uint8_t r = (uint8_t)(p % 12ULL);

            if (r == 1)   /* p ≡ 1 (mod 12) */
            {
                term = (seq_t)(exp + 1);
            }
            else if (r == 7)   /* p ≡ 7 (mod 12) */
            {
                if ((exp & 1) == 0)    /* even exponent */
                    term = (seq_t)(exp + 1);
                else
                    term = -((seq_t)(exp + 1));   /* negative sign as signed */
            }
            else if (r == 5 || r == 11)  /* p ≡ 5 or 11 (mod 12) */
            {
                if ((exp & 1) == 0)    /* even exponent gives 1 */
                    term = (seq_t)1;
                else
                    term = (seq_t)0;   /* odd exponent gives 0 */
            }
            else                          /* p == 2 or p == 3 (not possible) */
            {
                term = (seq_t)0;
            }

            result *= term;
            if (result == (seq_t)0)   /* once zero, remains zero */
                return result;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1)
    {
        uint64_t p = m;
        uint32_t exp = 1;   /* the leftover prime occurs exactly once */

        seq_t term;
        uint8_t r = (uint8_t)(p % 12ULL);

        if (r == 1)         /* p ≡ 1 (mod 12) */
            term = (seq_t)(exp + 1);
        else if (r == 7)    /* p ≡ 7 (mod 12) */
            term = -((seq_t)(exp + 1));
        else if (r == 5 || r == 11)  /* p ≡ 5 or 11 (mod 12) */
            term = (exp & 1) ? (seq_t)0 : (seq_t)1;
        else                 /* p == 2 or 3, shouldn't happen */
            term = (seq_t)0;

        result *= term;
    }

    return result;
}
