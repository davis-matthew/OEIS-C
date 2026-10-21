/*
 * A068970  a(n) = Sum_{d|n} phi(d^4).
 * https://oeis.org/A068970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = Sum_{k=1..n} (n/gcd(k,n))^3 Equivalent to a(n) =
 *            Sum_{d|n} d^3 * phi(d)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *primes,
                                   uint64_t *exps, size_t *cnt)           */
#include "../utils/calc_phi.h"    /* calc_phi(seq_t m)                      */

static void dfs_divisor_sum(uint64_t idx, uint64_t cur,
                            const uint64_t *primes, const uint64_t *exps,
                            size_t cnt, seq_t *accum)
{
    if (idx == cnt) {
        seq_t d = (seq_t)cur;
        seq_t term = d * d * d;          /* d^3 */
        term = term * calc_phi(d);       /* d^3 * phi(d) */
        *accum += term;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        dfs_divisor_sum(idx + 1, cur * mult, primes, exps, cnt, accum);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    /* Factor n */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    dfs_divisor_sum(0, 1, primes, exps, cnt, &result);
    return result;
}
