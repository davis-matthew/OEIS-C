/*
 * A058838  a(n) = 1 + sum of the anti-divisors of n.
 * https://oeis.org/A058838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum of anti‑divisors of n
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   factor_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An anti‑divisor of n is a quotient q = m/d with m ∈ {2n−1, 2n, 2n+1},
   d an odd divisor of m greater than 1, and q > 1. The sum is taken
   over all distinct q that arise from the three values of m. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_odd.h"

/* recursive generator of all divisors of the number represented by
 * the prime factorisation pr[0..cnt-1] with exponents exp[0..cnt-1].
 * divs[0..total-1] will hold the divisors; total is updated.
 */
static void gen_divisors(size_t idx,
                         uint32_t current,
                         const uint32_t *pr,
                         const uint32_t *exp,
                         size_t pf_cnt,
                         uint32_t *divs,
                         size_t *total)
{
    if (idx == pf_cnt) {
        divs[(*total)++] = current;
        return;
    }
    uint32_t val = current;
    for (uint32_t e = 0; e <= exp[idx]; ++e) {
        gen_divisors(idx + 1, val, pr, exp, pf_cnt, divs, total);
        val *= pr[idx];
    }
}

/* compute anti‑divisors of a single m (m > 0).  The numbers q are added
 * to *sum, avoiding duplicates within this call by using the auxiliary
 * array q_list.  q_list must have enough space for all possible q.  */
static void add_anti_divisors(uint64_t m,
                              uint64_t *sum,
                              uint64_t *q_list,
                              size_t *qcnt)
{
    /* odd part of m */
    uint32_t odd = (uint32_t)m;
    while ((odd & 1U) == 0) odd >>= 1;
    if (odd <= 1) return;          /* no odd divisor > 1 */

    /* factor the odd part */
    uint32_t pr[20], exp[20];
    size_t cnt;
    factor_odd(odd, pr, exp, &cnt);

    /* generate all divisors of the odd part */
    uint32_t divs[256];
    size_t total = 0;
    gen_divisors(0, 1, pr, exp, cnt, divs, &total);

    for (size_t i = 0; i < total; ++i) {
        uint32_t d = divs[i];
        if (d <= 1) continue;          /* only d > 1 */
        uint64_t q = m / d;            /* quotient */
        if (q <= 1) continue;          /* only q > 1 */
        /* check for duplicates within this call */
        size_t j;
        for (j = 0; j < *qcnt; ++j) if (q_list[j] == q) break;
        if (j == *qcnt) {
            q_list[(*qcnt)++] = q;
            *sum += q;
        }
    }
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    /* local array for collecting quotients (max 256 sufficient for n <= 10000) */
    uint64_t q_list[256];
    size_t qcnt = 0;
    uint64_t sum = 0;

    uint64_t m1 = 2ULL * (uint64_t)n - 1ULL;
    uint64_t m2 = 2ULL * (uint64_t)n;
    uint64_t m3 = 2ULL * (uint64_t)n + 1ULL;

    add_anti_divisors(m1, &sum, q_list, &qcnt);
    add_anti_divisors(m2, &sum, q_list, &qcnt);
    add_anti_divisors(m3, &sum, q_list, &qcnt);

    return (seq_t)sum + (seq_t)1;
}
