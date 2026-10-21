/*
 * A070032  Integer part of sigma(n)/phi(n).
 * https://oeis.org/A070032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor( sigma(n) / phi(n) )
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   sigma(n) = sum of divisors of n
    phi(n)   = Euler's totient of n
   The implementation factorises n by trial division and
   computes sigma and phi multiplicatively using only integer
   arithmetic.                                            */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return 0;

    idx_t m = n;                      /* mutable copy of n          */
    seq_t sigma = 1;
    seq_t phi   = 1;

    /* trial division over possible prime factors */
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            seq_t power = 1;          /* p^e */

            while (m % p == 0)
            {
                m /= p;
                ++e;
                power *= (seq_t)p;    /* p^e */
            }

            seq_t pseq      = (seq_t)p;
            seq_t powerNext = power * pseq;            /* p^(e+1)          */
            seq_t sigmaPiece = (powerNext - 1) / (pseq - 1);
            sigma *= sigmaPiece;

            seq_t phiPiece;
            if (e == 1)
                phiPiece = pseq - 1;
            else
            {
                seq_t powerPrev = power / pseq;        /* p^(e-1)        */
                phiPiece = powerPrev * (pseq - 1);
            }
            phi *= phiPiece;
        }
    }

    /* remaining prime factor > sqrt(n), if any */
    if (m > 1)
    {
        seq_t pseq = (seq_t)m;
        seq_t sigmaPiece = (pseq * pseq - 1) / (pseq - 1); /* = p+1        */
        sigma *= sigmaPiece;
        seq_t phiPiece = pseq - 1;
        phi *= phiPiece;
    }

    return sigma / phi;   /* integer division automatically floors */
}
