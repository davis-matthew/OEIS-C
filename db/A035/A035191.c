/*
 * A035191  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          9.
 * https://oeis.org/A035191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = τ(3n) – τ(n) = ∏_{p|n, p≠3} (exponent(p)+1) = number of
 *            divisors of n that are not divisible by 3
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>


seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n=0, just a safeguard */

    uint64_t m = (uint64_t)n;              /* local copy for factorisation */
    seq_t  res = 1;                        /* product of (e+1) for primes ≠ 3 */

    /* handle factor 2 separately */
    uint64_t c = 0;
    while (m % 2 == 0) {
        m /= 2;
        ++c;
    }
    if (c > 0) res *= (seq_t)(c + 1);

    /* trial division for odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (p != 3)                    /* skip powers of 3    */
                res *= (seq_t)(e + 1);     /* multiply (exponent+1) */
        }
    }

    /* remaining prime factor (> sqrt(original m)) */
    if (m > 1) {
        if (m != 3)                         /* single prime > sqrt, not 3 */
            res *= (seq_t)2;
        /* else m==3 : skip */
    }

    return res;
}
