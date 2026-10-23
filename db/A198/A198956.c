/*
 * A198956  q-expansion of modular form psi_0^4/t_{3B}.
 * https://oeis.org/A198956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is multiplicative with a(3^e) = 3^(3e) a(p^e) = 1 + p^3 +
 *            p^6 + … + p^(3e) for primes p ≠ 3
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 1;

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    /* decompose n into prime factors */
    factor_u64((uint64_t)n, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        if (p == 3) {
            /* a(3^e) = 3^(3e) */
            seq_t mult = pow_seq((seq_t)3, (int)(3 * e));
            result *= mult;
        } else {
            /* a(p^e) = 1 + p^3 + … + p^(3e) */
            seq_t p3 = pow_seq((seq_t)p, 3);
            seq_t sum = 1;
            seq_t term = p3;
            for (uint64_t j = 1; j <= e; ++j) {
                sum += term;
                if (j < e) term *= p3;
            }
            result *= sum;
        }
    }

    return result;
}
