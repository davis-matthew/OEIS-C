/*
 * A173493  Number of distinct squares that can be partitioned into distinct
 *          divisors of n.
 * https://oeis.org/A173493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct squares that can be partitioned into
 *            distinct divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming over the divisor sum space. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) /* offset is 1, but treat n==1 specially */
        return 1;

    /* 1. Factor n and generate all divisors */
    uint64_t rem = n;
    uint64_t primes[20];
    int exps[20];
    size_t pf_cnt = 0;

    if (rem % 2 == 0) {
        int e = 0;
        while (rem % 2 == 0) {
            rem /= 2;
            ++e;
        }
        primes[pf_cnt] = 2;
        exps[pf_cnt++] = e;
    }
    for (uint64_t p = 3; p * p <= (uint64_t)rem; p += 2) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt++] = e;
        }
    }
    if (rem > 1) {
        primes[pf_cnt] = rem;
        exps[pf_cnt++] = 1;
    }

    /* generate all divisors into an array */
    size_t divs_cap = 2000;                    /* ample capacity for n <= 10000 */
    uint64_t *divs = (uint64_t *)calloc(divs_cap, sizeof(uint64_t));
    if (!divs) return 0;
    size_t divc = 1;
    divs[0] = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = primes[i];
        int e = exps[i];
        size_t cur_cnt = divc;
        for (size_t j = 0; j < cur_cnt; ++j) {
            uint64_t base = divs[j];
            for (int k = 1; k <= e; ++k) {
                base *= p;
                divs[divc++] = base;
            }
        }
    }

    /* 2. Compute sigma (sum of divisors) */
    size_t sigma = 0;
    for (size_t i = 0; i < divc; ++i)
        sigma += divs[i];

    /* 3. Subset-sum DP over {0 .. sigma} */
    uint8_t *reach = (uint8_t *)calloc(sigma + 1, 1);
    if (!reach) {
        free(divs);
        return 0;
    }
    reach[0] = 1;
    for (size_t d_idx = 0; d_idx < divc; ++d_idx) {
        uint64_t d = divs[d_idx];
        if (d > sigma) continue;
        size_t di = (size_t)d;
        for (size_t s = sigma; s >= di; --s) {
            if (reach[s - di] != 0)
                reach[s] = 1;
        }
    }

    /* 4. Count perfect squares reachable */
    uint64_t count = 0;
    for (uint64_t m = 1; m * m <= sigma; ++m) {
        size_t sq = (size_t)(m * m);
        if (reach[sq]) ++count;
    }

    free(reach);
    free(divs);
    return (seq_t)count;
}
