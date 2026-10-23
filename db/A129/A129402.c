/*
 * A129402  Expansion of phi(x^3) * psi(x^4) + x * phi(x) * psi(x^12) in
 *          powers of x where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A129402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A129402
 * a(n) = b(2*n+1) where b is multiplicative with
 *   b(2^e) = 0^e  (i.e. b(m)=0 if m even),
 *   b(3^e) = 1,
 *   b(p^e) = e+1 if p ≡ 1,5,7,11 (mod 24),
 *   b(p^e) = (1+(-1)^e)/2 if p ≡ 13,17,19,23 (mod 24).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 2*n+1 (always odd) */
    seq_t m = (seq_t)n * 2 + 1;

    /* Special case m = 1 */
    if (m == 1) return 1;

    seq_t t = m;          /* working copy for factorisation */
    seq_t result = 1;     /* product of (e+1) for admissible primes */

    /* Trial division by odd numbers starting at 3 */
    for (seq_t p = 3; p <= t / p; p += 2)
    {
        if (t % p == 0)
        {
            seq_t exp = 0;
            while (t % p == 0)
            {
                t /= p;
                ++exp;
            }

            if (p == 3)        /* prime 3 contributes 1 */
                continue;

            seq_t mod = p % 24;

            /* primes ≡ 13,17,19,23 (mod 24) must have even exponent */
            if (mod == 13 || mod == 17 || mod == 19 || mod == 23)
            {
                if (exp & 1)   /* odd exponent → result zero */
                    return 0;
                /* even exponent contributes factor 1 */
                continue;
            }

            /* primes ≡ 1,5,7,11 (mod 24) contribute (e+1) */
            if (mod == 1 || mod == 5 || mod == 7 || mod == 11)
            {
                result *= (exp + 1);
            }
            /* all other primes cannot divide an odd number >1 */
        }
    }

    /* Remaining factor t > 1 is a prime */
    if (t > 1)
    {
        seq_t p = t;
        seq_t exp = 1;          /* exponent is 1 */

        if (p == 3)
        {
            /* no effect on result */
        }
        else
        {
            seq_t mod = p % 24;
            if (mod == 13 || mod == 17 || mod == 19 || mod == 23)
                return 0;
            if (mod == 1 || mod == 5 || mod == 7 || mod == 11)
                result *= 2;   /* exp + 1 = 2 */
        }
    }

    return result;
}
