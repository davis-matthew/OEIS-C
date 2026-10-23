/*
 * A035182  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s) + Kronecker(m,p)*p^(-2s))^(-1) for m
 *          = -7.
 * https://oeis.org/A035182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative; a(7^e)=1, a(p^e)=e+1 if p≡1,2,4 (mod 7),
 *            a(p^e)=(1+(-1)^e)/2 if p≡3,5,6 (mod 7)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Maximum number of distinct prime factors of a 64‑bit integer is < 64. */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == (uint64_t)7)
            continue;

        uint64_t mod = p % (uint64_t)7;
        if (mod == (uint64_t)1 || mod == (uint64_t)2 || mod == (uint64_t)4) {
            result *= (seq_t)(e + (uint64_t)1);
        } else if (mod == (uint64_t)3 || mod == (uint64_t)5 || mod == (uint64_t)6) {
            if ((e & (uint64_t)1) == (uint64_t)1)
                return (seq_t)0;              /* odd exponent gives 0 */
            /* even exponent gives factor 1 – nothing to multiply */
        }
        /* mod == 0 cannot occur except for p == 7, handled above */
    }
    return result;
}
