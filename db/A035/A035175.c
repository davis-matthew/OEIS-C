/*
 * A035175  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -15.
 * https://oeis.org/A035175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative. For n = ∏p^e: - a(3^e)=a(5^e)=1 - if p ≡
 *            7,11,13,14 (mod 15): a(p^e) = (1+(-1)^e)/2 (1 if e even, 0 if e
 *            odd) - if p ≡ 1,2,4,8 (mod 15): a(p^e) = e+1 (with a(1^e)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the prime‑power rules from the OEIS entry. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for 0, but return 0 just in case */

    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        unsigned mod = (unsigned)(p % 15u);

        if (p == 3 || p == 5) {
            /* value is 1; nothing changes */
            continue;
        }

        if (mod == 7u || mod == 11u || mod == 13u || mod == 14u) {
            if (e & 1u)   /* odd exponent */
                return (seq_t)0;
            /* even exponent gives 1; nothing to multiply */
            continue;
        }

        /* p ≡ 1,2,4,8 (mod 15) */
        if (mod == 1u || mod == 2u || mod == 4u || mod == 8u) {
            prod = prod * ((seq_t)(e + 1u));
            continue;
        }

        /* sanity for any unexpected residue (primes are covered above) */
        prod = prod * ((seq_t)(e + 1u));
    }

    return prod;
}
