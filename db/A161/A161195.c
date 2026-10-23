/*
 * A161195  a(n) = ((2^b-1)/phi(n))*Sum_{d|n} Moebius(n/d)*d^(b-1) for b = 16.
 * https://oeis.org/A161195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2^16-1)/phi(n)) * Σ_{d|n} μ(n/d) * d^15 For b = 16,
 *            this equals 65535 * Π_{p^e||n} p^{(e-1)*14} * (1 + p + … + p^14)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 10, u128 n <= 239,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)65535;          /* constant (2^16-1) */
    if (n == 0) return 0;                 /* behavior below offset irrelevant */

    /* factorize n; store primes and exponents */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* compute S_p = (p^15 - 1) / (p - 1) = 1 + p + ... + p^14 */
        seq_t pow15 = pow_uint(p, (idx_t)15);   /* p^15 */
        seq_t sum   = (pow15 - (seq_t)1) / ((seq_t)(p - 1));

        result *= sum;                /* multiply by S_p */

        /* multiply by p^{(e-1)*14} if e >= 1 */
        if (e >= 1) {
            idx_t exp14 = (idx_t)(e - 1) * (idx_t)14;
            seq_t pow_p = pow_uint(p, exp14);
            result *= pow_p;
        }
    }

    return result;
}
