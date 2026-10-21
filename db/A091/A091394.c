/*
 * A091394  a(n) = Product_{ p | n } (1 + Legendre(-5,p) ).
 * https://oeis.org/A091394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{ p | n } (1 + Legendre(-5,p) )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow_mod_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_mod_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined for n=0, but return 0 */
        return 0;
    uint64_t m = (uint64_t)n;

    /* Even numbers contain the prime 2, and Legendre(-5,2) = -1,
       hence the product becomes zero. */
    if ((m & 1ULL) == 0ULL)
        return 0;

    seq_t result = 1;                /* product over distinct odd primes */

    /* Factorize m by trial division on odd primes. */
    for (uint64_t p = 3; p * p <= m; p += 2)
    {
        if (m % p == 0)
        {
            /* Remove the entire power of p (only need distinct primes). */
            while (m % p == 0)
                m /= p;

            /* Compute Legendre symbol (−5 / p). */
            uint64_t a_coef = (p - (5 % p)) % p;   /* (−5 mod p) */
            uint64_t exp = (p - 1) >> 1;           /* (p−1)/2 */
            uint64_t leg = pow_mod_uint64(a_coef, exp, p);

            if (leg == p - 1)                /* Legendre = −1 */
                return 0;
            else if (leg == 1)                /* Legendre = +1 */
                result = result + result;     /* multiply by 2 */
            /* leg == 0 (possible only for p = 5) → factor ≈ 1, no change */
        }
    }

    /* Remaining prime factor > sqrt(original). */
    if (m > 1)
    {
        uint64_t p = m;
        uint64_t a_coef = (p - (5 % p)) % p;
        uint64_t exp = (p - 1) >> 1;
        uint64_t leg = pow_mod_uint64(a_coef, exp, p);

        if (leg == p - 1)
            return 0;
        else if (leg == 1)
            result = result + result;     /* multiply by 2 */
    }

    return result;
}
