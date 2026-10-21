/*
 * A220849  a(n) = Product_{d|n} Product_{d_x|n , d_x <= d} d_x.
 * https://oeis.org/A220849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} Product_{d_x|n, d_x <= d} d_x (OEIS
 *            A220849)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 35, u128 n <= 59,
 *            big any n
 * Helpers:   divisor_count_u64, factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/seq_mul.h"

static void gen_divs_rec(size_t idx, uint64_t cur,
                         const uint64_t *primes,
                         const uint64_t *exps,
                         size_t pf_cnt, uint64_t *out, size_t *cnt)
{
    if (idx == pf_cnt) {
        out[(*cnt)++] = cur;
        return;
    }
    uint64_t val = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divs_rec(idx + 1, cur * val, primes, exps, pf_cnt, out, cnt);
        val *= primes[idx];
    }
}

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va > vb) - (va < vb);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                /* undefined for n < 1 */

    /* Prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* Number of divisors of n */
    size_t div_cnt = divisor_count_u64((uint64_t)n);
    uint64_t *divs = (uint64_t *)malloc(div_cnt * sizeof(uint64_t));
    if (!divs) return (seq_t)0;
    size_t cur_cnt = 0;

    /* Enumerate all divisors */
    gen_divs_rec(0, 1ULL, primes, exps, pf_cnt, divs, &cur_cnt);

    /* Sort divisors in ascending order */
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_uint64);

    /* Compute the product a(n) = Π d_i^{τ(n)-i+1}   */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < div_cnt; ++i) {
        seq_t base = (seq_t)divs[i];
        size_t exp = div_cnt - i;            /* number of times to multiply */
        for (size_t k = 0; k < exp; ++k)
            result = seq_mul(result, base);
    }

    free(divs);
    return result;
}
