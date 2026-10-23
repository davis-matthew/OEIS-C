/*
 * A013968  a(n) = sigma_20(n), the sum of the 20th powers of the divisors of
 *          n.
 * https://oeis.org/A013968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = σ₍₂₀₎(n) = Σ_{d|n} d^20 (OEIS A013968)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 84,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* offset is 1, but we return 0 for n=0 */
    if (n == 1)
        return 1;              /* σ₍₂₀₎(1) = 1 */

    /* Factorise n into primes (pᵉ) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        unsigned int e = (unsigned int)exps[i];

        /* Compute p^20 once */
        seq_t p20 = 1;
        for (unsigned int j = 0; j < 20; ++j)
            p20 *= p;

        /* σ₍₂₀₎(pᵉ) = 1 + p^20 + p^40 + … + p^(20e) */
        seq_t term = 1;
        seq_t pow = p20;          /* current p^(20k), starting with k=1 */
        for (unsigned int k = 0; k < e; ++k) {
            term += pow;
            pow *= p20;          /* advance to next power of p^20 */
        }

        result *= term;
    }

    return result;
}
