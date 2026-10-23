/*
 * A169594  Number of divisors of n, counting divisor multiplicity in n.
 * https://oeis.org/A169594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + \sum_{d|n,\,d>1} \min_{p|d} \left\lfloor
 *            \frac{e_p}{f_p}\right\rfloor * where n=\prod_p p^{e_p} and
 *            d=\prod_p p^{f_p}. The divisor 1 has multiplicity 1 by
 *            definition
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static void dfs(size_t idx, size_t nb, const uint64_t *exp, uint64_t *f, seq_t *total)
{
    if (idx == nb) {
        /* divisor > 1 ? */
        size_t i;
        for (i = 0; i < nb; ++i)
            if (f[i] != 0) break;
        if (i == nb) return;  /* divisor is 1 */

        /* compute multiplicity = min floor(exp[i]/f[i]) over f[i]>0 */
        unsigned long long min_e = ~0ULL;
        for (i = 0; i < nb; ++i) {
            if (f[i] != 0) {
                unsigned long long val = exp[i] / f[i];
                if (val < min_e) min_e = val;
            }
        }
        *total += (seq_t)min_e;
        return;
    }

    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        f[idx] = e;
        dfs(idx + 1, nb, exp, f, total);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t nb = 0;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[nb] = p;
            exps[nb] = e;
            ++nb;
        }
    }
    if (m > 1) {
        primes[nb] = m;
        exps[nb] = 1;
        ++nb;
    }

    /* enumerate divisors and sum multiplicities */
    seq_t total = 0;
    uint64_t f[64] = {0};
    dfs(0, nb, exps, f, &total);

    /* add multiplicity of divisor 1 */
    return total + (seq_t)1;
}
