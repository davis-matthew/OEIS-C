/*
 * A152958  Alladi's third-order function phi_3(n).
 * https://oeis.org/A152958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, look
 * Formula:   a(n) = Sum_{d^2 | n} phi(n/d)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    if (n == 0) return 0;          /* undefined for n < 1 */
    if (n == 1) return 1;          /* a(1) = 1 */

    uint64_t num = (uint64_t)n;
    size_t cnt;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(num, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* p^e */
        seq_t p_pow_e = 1;
        for (uint64_t j = 0; j < e; ++j) {
            p_pow_e *= (seq_t)p;
        }

        /* k = ceil((e+1)/2) = (e+2)/2 */
        uint64_t k = (e + 2) / 2;
        uint64_t e_minus_k = e - k;

        /* p^(e-k) */
        seq_t p_pow_rem = 1;
        for (uint64_t j = 0; j < e_minus_k; ++j) {
            p_pow_rem *= (seq_t)p;
        }

        seq_t term = p_pow_e - p_pow_rem;
        result *= term;
    }
    return result;
}
