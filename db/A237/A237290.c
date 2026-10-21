/*
 * A237290  Sum of positive numbers k <= sigma(n) that are a sum of any subset
 *          of distinct divisors of n.
 * https://oeis.org/A237290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of distinct subset sums of all divisors of n The sum
 *            is taken over all positive numbers k <= σ(n) that can be
 *            obtained as the sum of a subset of the distinct divisors of n
 * Range:     u8 n <= 11, u16 n <= 143, u32 n <= 7942, u64 n <= 7942,
 *            u128 n <= 7942, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Factor n into primes */
    idx_t rem = n;
    idx_t primes[20];
    int exps[20];
    size_t fact_cnt = 0;
    for (idx_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            primes[fact_cnt] = p;
            exps[fact_cnt] = e;
            ++fact_cnt;
        }
    }
    if (rem > 1) {
        primes[fact_cnt] = rem;
        exps[fact_cnt] = 1;
        ++fact_cnt;
    }

    /* Compute σ(n) */
    idx_t sigma = 1;
    for (size_t i = 0; i < fact_cnt; ++i) {
        idx_t p = primes[i];
        int e = exps[i];
        idx_t pk = 1;
        for (int k = 0; k < e + 1; ++k) pk *= p;
        sigma *= (pk - 1) / (p - 1);
    }

    /* Generate all divisors of n */
    size_t divs_cnt = 1;
    idx_t *divs = (idx_t *)malloc(sizeof(idx_t));
    if (!divs) return 0;           /* allocation failure */
    divs[0] = 1;
    for (size_t i = 0; i < fact_cnt; ++i) {
        idx_t p = primes[i];
        int e = exps[i];
        size_t old_cnt = divs_cnt;
        size_t new_cnt = old_cnt * (e + 1);
        idx_t *new_divs = (idx_t *)malloc(new_cnt * sizeof(idx_t));
        if (!new_divs) { free(divs); return 0; }
        idx_t mult = 1;
        for (int k = 0; k <= e; ++k) {
            for (size_t j = 0; j < old_cnt; ++j) {
                new_divs[j * (e + 1) + k] = divs[j] * mult;
            }
            mult *= p;
        }
        free(divs);
        divs = new_divs;
        divs_cnt = new_cnt;
    }

    /* Boolean DP for achievable subset sums */
    size_t sigma_sz = (size_t)sigma + 1;
    unsigned char *reach = (unsigned char *)calloc(sigma_sz, 1);
    if (!reach) { free(divs); return 0; }
    reach[0] = 1;
    for (size_t idx = 0; idx < divs_cnt; ++idx) {
        idx_t d = divs[idx];
        int64_t d64 = (int64_t)d;
        for (int64_t s = (int64_t)sigma; s >= d64; --s) {
            if (reach[(size_t)(s - d64)]) {
                reach[(size_t)s] = 1;
            }
        }
    }

    /* Sum all positive achievable sums */
    seq_t total = 0;
    for (size_t k = 1; k < sigma_sz; ++k) {
        if (reach[k]) total += (seq_t)k;
    }

    free(reach);
    free(divs);
    return total;
}
