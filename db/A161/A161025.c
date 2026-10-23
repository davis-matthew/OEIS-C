/*
 * A161025  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 15.
 * https://oeis.org/A161025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = J_14(n)/phi(n) where J_14(n) = prod_{p^e||n} p^(14e)
 *            (1-1/p^14) and phi(n) = J_1(n) = prod_{p^e||n} p^e (1-1/p) Hence
 *            a(n) = prod_{p^e||n} p^(13e-13) * (p^14-1)/(p-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 29, u128 n <= 839,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   This equals prod_{p^e||n} p^(13(e-1)) * sum_{i=0}^{13} p^i   (with empty product = 1) */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* function: void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n)
{
    /* Handle n = 0 or below offset – behaviour not required */
    if (n == 0) return (seq_t)0;    /* placeholder, not needed */

    /* factor n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t product = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = pr[i];
        uint64_t e  = ex[i];

        /* Compute p^(13*(e-1)) */
        seq_t pow_factor = (seq_t)1;
        uint64_t exp = 13ULL * (e > 0 ? (e - 1) : 0);
        for (uint64_t k = 0; k < exp; ++k)
            pow_factor *= (seq_t)p;

        /* Compute sigma_13(p) = sum_{i=0}^{13} p^i */
        seq_t sigma = (seq_t)1;   /* p^0 */
        seq_t cur   = (seq_t)1;
        for (uint64_t k = 1; k <= 13; ++k) {
            cur *= (seq_t)p;      /* p^k */
            sigma += cur;
        }

        /* factor for this prime power */
        seq_t factor = pow_factor * sigma;
        product *= factor;
    }

    return product;
}
