/*
 * A077567  Least k >= 2 such that sigma(n) divides sigma(n^k).
 * https://oeis.org/A077567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 2 such that σ(n) divides σ(n^k)
 * Range:     u8 n <= 2048, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes σ(n) and then checks the condition
 * for successive k by performing modular arithmetic only.
 * The sum Σ_{i=0}^{E} p^i is computed modulo σ(n) to avoid
 * overflow.  The product of these partial sums is kept modulo σ(n),
 * and a zero residue implies divisibility.  Factorisation of n
 * is done once per call using the supplied factor_u64 helper. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Special case: n = 1 */
    if (n == 1) {
        return 2;
    }

    /* Retrieve prime factorisation of n.  The helper writes exponents
     * into the exps array in the same order as primes. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute σ(n) once.  It fits in seq_t. */
    seq_t sigma_n = sigma_of((uint64_t)n);

    /* Search for the minimal k ≥ 2. */
    for (seq_t k = 2;; ++k) {
        seq_t prod_mod = 1 % sigma_n;   /* product of partial sums modulo σ(n) */

        /* Process each prime factor. */
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p   = primes[i];
            uint64_t e   = exps[i];
            uint64_t E   = e * (uint64_t)k;        /* exponent e*k */

            /* Compute S = Σ_{i=0}^{E} p^i mod σ(n).  Do this
             * without exceeding σ(n) by taking mods at each step. */
            seq_t base   = (seq_t)(p % sigma_n);
            seq_t term   = 1 % sigma_n;          /* p^0 */
            seq_t sum_mod = term;                /* start with i=0 */

            for (uint64_t j = 1; j <= E; ++j) {
                term = (term * base) % sigma_n;  /* p^j */
                sum_mod = (sum_mod + term) % sigma_n;
            }

            /* Multiply into the running product modulo σ(n). */
            prod_mod = (prod_mod * sum_mod) % sigma_n;

            /* Early exit: if product already zero we know divisibility. */
            if (prod_mod == 0) {
                break;
            }
        }

        /* After processing all prime factors, check divisibility. */
        if (prod_mod == 0) {
            return k;
        }
    }
}
