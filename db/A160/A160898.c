/*
 * A160898  a(n) = ((2^b-1)/phi(n))*Sum_{d|n} Moebius(n/d)*d^(b-1) for b = 7.
 * https://oeis.org/A160898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 127 * ∏_{p^e‖n} p^(5e-5) * (1 + p + p^2 + p^3 + p^4 +
 *            p^5)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 27, u64 n <= 2069,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* simple integer power for seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset 1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 127;    /* factor 127 for n = 1 and for each prime factor */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        idx_t exp = (idx_t)(5 * e - 5);          /* exponent 5e-5 */
        seq_t term = pow_seq_t(p, exp);          /* p^(5e-5) */

        /* geometric sum 1 + p + … + p^5 */
        seq_t sum = 1;
        seq_t powp = 1;
        for (int k = 1; k <= 5; ++k) {
            powp *= p;
            sum += powp;
        }

        term *= sum;    /* p^(5e-5) * (1+p+…+p^5) */
        result *= term;
    }

    return result;
}
