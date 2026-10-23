/*
 * A167315  Totally multiplicative sequence with a(p) = 5*(p-3) for prime p.
 * https://oeis.org/A167315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = 0 if 3 divides n; otherwise a(n) is the product over
 *            prime powers p^e||n of (5*(p-3))^e. This yields the totally
 *            multiplicative sequence with a(p^e) = (5*(p-3))^e
 * Range:     i8 n <= 15, i16 n <= 127, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == (idx_t)1) return (seq_t)1;

    /* a(3k) = 0 for k ≥ 1 */
    if (n % (idx_t)3 == (idx_t)0) return (seq_t)0;

    seq_t result = (seq_t)1;
    uint64_t nn = (uint64_t)n;

    /* factor n by trial division */
    while (nn > 1)
    {
        uint64_t p = 0;

        /* find a prime factor */
        if ((nn & 1) == 0)                     /* divisible by 2 */
            p = 2;
        else
        {
            for (uint64_t k = 3; k * k <= nn; k += 2)
            {
                if (nn % k == 0) { p = k; break; }
            }
        }

        if (p == 0)          /* nn itself is prime */
        {
            p = nn;
            nn = 1;
        }
        else
        {
            /* count exponent e of this prime factor */
            int e = 0;
            while (nn % p == 0)
            {
                nn /= p;
                ++e;
            }
            /* add contribution of p^e */
            seq_t base = (seq_t)5 * ((seq_t)p - (seq_t)3);
            for (int i = 0; i < e; ++i)
                result *= base;
            continue; /* proceed to the next factor */
        }

        /* the case where p == nn (prime factor with exponent 1) */
        seq_t base = (seq_t)5 * ((seq_t)p - (seq_t)3);
        result *= base;
    }

    return result;
}
