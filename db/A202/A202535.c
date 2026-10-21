/*
 * A202535  a(n) = n*phi(n)*abs( mobius(n) ).
 * https://oeis.org/A202535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n*phi(n)*abs(mobius(n))
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: test square‑freeness via trial factorisation and
   multiply n by phi(n) if square‑free (abs(mobius)=1).  Uses the
   provided utility functions: factor_u64 and phi. */

#include "../utils/factor_u64.h"
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Factor n and check that no exponent exceeds 1 (square‑free). */
    /* We only need a modest number of primes; 64 is ample for n <=
       2^64. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(n, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] > 1)          /* n is divisible by p^2, not square‑free */
            return 0;
    }

    /* n is square‑free → |mu(n)| = 1 */
    seq_t phi_n = phi(n);
    return (seq_t)n * phi_n;
}
