/*
 * A206028  a(n) is the sum of distinct values of sigma(d) where d runs over
 *          the divisors of n and sigma = A000203.
 * https://oeis.org/A206028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct sigma(d) for all divisors d of n, where
 *            sigma is the sum‑of‑divisors function (A000203)
 * Range:     u8 n <= 47, u16 n <= 6299, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t, factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/sigma_of.h"   /* sigma_of(idx_t n) : returns seq_t */
#include "../utils/cmp_seq_t.h"  /* cmp_seq_t(const void *p1, const void *p2) */

/* recursively generate all divisors of n from its prime factorization */
static void gen_divisors(size_t idx, uint64_t cur,
                         const uint64_t *pr, const uint64_t *ex, size_t pf_cnt,
                         uint64_t *divs, size_t *div_cnt)
{
    if (idx == pf_cnt) {
        divs[(*div_cnt)++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (uint64_t i = 0; i <= ex[idx]; ++i) {
        gen_divisors(idx + 1, cur * val, pr, ex, pf_cnt, divs, div_cnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* factor n */
    uint64_t pr[64], ex[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &pf_cnt);

    /* generate all divisors of n */
    uint64_t divisors[256];
    size_t div_cnt = 0;
    gen_divisors(0, 1, pr, ex, pf_cnt, divisors, &div_cnt);

    /* compute sigma for each divisor and store in array */
    seq_t sigma_vals[256];
    size_t sigma_cnt = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        sigma_vals[sigma_cnt++] = sigma_of((idx_t)divisors[i]);
    }

    /* sort sigma_values to identify duplicates */
    qsort(sigma_vals, sigma_cnt, sizeof(seq_t),
          (int(*)(const void*, const void*))cmp_seq_t);

    /* sum distinct values */
    seq_t sum = 0;
    for (size_t i = 0; i < sigma_cnt; ++i) {
        if (i == 0 || cmp_seq_t(&sigma_vals[i], &sigma_vals[i - 1]) != 0) {
            sum += sigma_vals[i];
        }
    }
    return sum;
}
