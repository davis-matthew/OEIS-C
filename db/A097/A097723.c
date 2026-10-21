/*
 * A097723  One fourth of sum of divisors of 4n+3.
 * https://oeis.org/A097723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(4*n + 3) / 4 where sigma(x) is the sum of all
 *            positive divisors of x
 * Range:     u8 n <= 3, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute m = 4*n + 3 */
    uint64_t m = (uint64_t)n * 4ULL + 3ULL;

    /* factor m */
    uint64_t primes[64];   /* enough for all prime factors of m up to 64 bits */
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    /* compute sigma(m) */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        seq_t term_num = pow_uint(p, e + 1) - (seq_t)1;
        seq_t term_den = (seq_t)p - (seq_t)1;
        seq_t term = term_num / term_den;
        sigma *= term;
    }

    /* divide by 4 */
    return sigma / (seq_t)4;
}
