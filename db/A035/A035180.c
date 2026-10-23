/*
 * A035180  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -10.
 * https://oeis.org/A035180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   kronecker_minus10_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A035180
 *
 *  a(n) = Σ_{d|n} Kronecker(-10,d)
 *        = Π_{p^e‖n} a(p^e)
 *
 *  a(p^e) = 1                     if Kronecker(-10,p)=0   (p=2 or 5)
 *           (1+(-1)^e)/2          if Kronecker(-10,p)=-1
 *           e+1                   if Kronecker(-10,p)=1
 *
 * The function below implements this formula using trial division
 * for factorisation; it works for any idx_t (uint64_t) and any
 * seq_t (unsigned integer types up to 128 bits or arbitrary precision).
 */

#include <stdint.h>
#include "../utils/kronecker_minus10_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n < 1, guard anyway */

    uint64_t m = n;
    seq_t result = 1;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        /* factor p out of m */
        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }

        /* compute contribution of p^exp */
        int sym = kronecker_minus10_prime(p);
        seq_t mult;

        if (sym == 0)            /* 2 or 5 */
            mult = 1;
        else if (sym == -1)      /* p in A296925 */
            mult = (exp % 2 == 0) ? 1 : 0;   /* (1+(-1)^e)/2 */
        else                     /* sym == 1 */
            mult = (seq_t)(exp + 1);

        result *= mult;
    }

    /* if a prime larger than sqrt(n) remains */
    if (m > 1) {
        /* remaining factor is prime, exponent 1 */
        int sym = kronecker_minus10_prime(m);
        seq_t mult;

        if (sym == 0)
            mult = 1;
        else if (sym == -1)
            mult = 0;          /* exp=1 → (1+(-1))/2 = 0 */
        else
            mult = 2;          /* exp=1 → e+1 = 2 */

        result *= mult;
    }

    return result;
}
