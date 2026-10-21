/*
 * A237289  Sum of positive numbers k <= sigma(n) that are not a sum of any
 *          subset of distinct divisors of n.
 * https://oeis.org/A237289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237289 (OEIS)
   a(n) = sum of all integers k (1 ≤ k ≤ σ(n)) that cannot be expressed as a sum of a subset of distinct divisors of n
   σ(n) is the sum‑of‑divisors function.  Implementation uses a subset‑sum DP
   over the divisors of n.  The function works for any n ≥ 1; for small n
   (≤ 10 000) it runs in well below 2 s.  It is written so that it is
   valid C11 and C++17. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* the sequence is offset 1 */

    /* ------------------------------------------- */
    /* factorise n */
    /* ------------------------------------------- */
    uint64_t num = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);

    /* ------------------------------------------- */
    /* generate all divisors of n */
    /* ------------------------------------------- */
    size_t div_cap = 8;
    size_t dcnt = 1;
    uint64_t *divs = (uint64_t *)malloc(div_cap * sizeof(uint64_t));
    if (!divs) return 0;                     /* allocation failure */
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t mult = 1;
        size_t old_cnt = dcnt;
        for (uint64_t e = 1; e <= exps[i]; ++e) {
            mult *= p;
            for (size_t j = 0; j < old_cnt; ++j) {
                if (dcnt == div_cap) {
                    div_cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(divs, div_cap * sizeof(uint64_t));
                    if (!tmp) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[dcnt++] = divs[j] * mult;
            }
        }
    }

    /* ------------------------------------------- */
    /* compute σ(n) as a 64‑bit integer */
    /* ------------------------------------------- */
    uint64_t sigma = 0;
    for (size_t i = 0; i < dcnt; ++i) sigma += divs[i];

    /* ------------------------------------------- */
    /* DP: which sums of distinct divisors are reachable */
    /* ------------------------------------------- */
    size_t smax = (size_t)sigma;
    bool *reach = (bool *)calloc(smax + 1, sizeof(bool));
    if (!reach) { free(divs); return 0; }
    reach[0] = true;

    for (size_t i = 0; i < dcnt; ++i) {
        size_t d = (size_t)divs[i];
        for (size_t s = smax; ; --s) {
            if (reach[s - d]) reach[s] = true;
            if (s == d) break;
        }
    }

    /* ------------------------------------------- */
    /* sum all k that are NOT reachable */
    /* ------------------------------------------- */
    seq_t res = 0;
    for (size_t k = 1; k <= smax; ++k) {
        if (!reach[k]) res += (seq_t)k;
    }

    free(reach);
    free(divs);
    return res;
}
