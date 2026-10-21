/*
 * A064944  a(n) = Sum_{i|n, j|n, j >= i} j.
 * https://oeis.org/A064944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i|n, j|n, j >= i} j Computation: generate all
 *            divisors of n, sort them in ascending order, then compute
 *            Σ_{k=1}^{τ(n)} k * d_k where d_k are the sorted divisors
 * Range:     u8 n <= 23, u16 n <= 839, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t idx = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            pr[idx] = p;
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            exp[idx] = e;
            ++idx;
        }
    }
    if (n > 1) {
        pr[idx] = n;
        exp[idx] = 1;
        ++idx;
    }
    *cnt = idx;
}

static void gen_divs_rec(size_t idx, uint64_t cur,
                         const uint64_t *pr, const uint64_t *exp, size_t cnt,
                         uint64_t *divs, size_t *size)
{
    if (idx == cnt) {
        divs[(*size)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (uint64_t k = 0; k <= exp[idx]; ++k) {
        gen_divs_rec(idx + 1, val, pr, exp, cnt, divs, size);
        val *= pr[idx];
    }
}

static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* sequence is defined for n >= 1 */
    uint64_t num = (uint64_t)n;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);

    /* Compute total number of divisors */
    size_t total = 1;
    for (size_t i = 0; i < cnt; ++i)
        total *= (size_t)(exps[i] + 1);

    /* Generate all divisors */
    uint64_t *divs = (uint64_t *)malloc(total * sizeof(uint64_t));
    size_t sz = 0;
    gen_divs_rec(0, 1ULL, primes, exps, cnt, divs, &sz);

    /* Sort divisors */
    qsort(divs, sz, sizeof(uint64_t), cmp_uint64);

    /* Compute the sum */
    seq_t sum = 0;
    for (size_t i = 0; i < sz; ++i) {
        sum += (seq_t)(i + 1) * (seq_t)divs[i];
    }

    free(divs);
    return sum;
}
