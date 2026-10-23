/*
 * A174476  a(n) = Sum_{d|n} d^phi(d).
 * https://oeis.org/A174476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^phi(d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, calc_phi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/calc_phi.h"

/* Recursive helper: sum over divisors constructed from prime factorization. */
static seq_t rec_div_sum(int i, seq_t cur, uint64_t *primes, uint64_t *exps, int cnt)
{
    if (i == cnt) {
        seq_t phi = calc_phi(cur);
        return big_pow(cur, phi);
    }

    seq_t sum = 0;
    seq_t d = cur;
    for (uint64_t e = 0; e <= exps[i]; ++e) {
        sum += rec_div_sum(i + 1, d, primes, exps, cnt);
        d = d * (seq_t)primes[i];
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, return 0 for safety */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    return rec_div_sum(0, (seq_t)1, primes, exps, (int)cnt);
}
