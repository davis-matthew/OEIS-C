/*
 * A167363  Totally multiplicative sequence with a(p) = (p+3)^2 = p^2+6p+9 for
 *          prime p.
 * https://oeis.org/A167363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e∥n} (p+3)^{2e} (multiplicative, a(p)=(p+3)^2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Computes the A167363 sequence value for any n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 1)                /* no prime factors ⇒ product = 1 */
        return 1;

    /* Temporary buffers for prime factors of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64(n, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base  = (seq_t)(primes[i] + 3);
        seq_t base2 = base * base;      /* (p+3)^2 */

        seq_t term = 1;
        for (size_t e = 0; e < exps[i]; ++e)
            term *= base2;              /* (p+3)^(2e) for this prime */

        prod *= term;
    }
    return prod;
}
