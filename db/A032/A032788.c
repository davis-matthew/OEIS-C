/*
 * A032788  Numbers k such that k(k+1)(k+2)...(k+19) /
 *          (k+(k+1)+(k+2)+...+(k+19)) is an integer.
 * https://oeis.org/A032788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (d-19)/2 where d >= 19 and d | 85734032330071125. The
 *            sequence consists of all k obtained from the divisors d of that
 *            constant
 * Range:     u8 n <= 1, u16 n <= 30, u32 n <= 2589, u64 n <= 8739,
 *            u128 n <= 8739, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Static cache for the sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (!cache) {
        /* 85734032330071125 = 19 · 23 · 31 · 37 · 71 · 101 · 241 · 991
           The following code factorizes it and enumerates all its
           divisors.  For each divisor d ≥ 19 we store (d-19)/2 in
           cache. */
        const uint64_t D = 85734032330071125ULL;

        uint64_t primes[64], exps[64];
        size_t cnt;
        factor_u64(D, primes, exps, &cnt);

        /* Count total number of divisors. */
        size_t total = 1;
        for (size_t i = 0; i < cnt; ++i) {
            total *= (exps[i] + 1);
        }

        uint64_t *divs = (uint64_t *)malloc(sizeof(uint64_t) * total);
        if (!divs) return 0;            /* allocation failure */
        divs[0] = 1;
        size_t cur = 1;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            size_t exp = exps[i];
            size_t prev = cur;
            uint64_t pow = 1;
            for (size_t k = 1; k <= exp; ++k) {
                pow *= p;
                for (size_t j = 0; j < prev; ++j) {
                    divs[cur++] = divs[j] * pow;
                }
            }
        }

        /* Count how many k values (i.e., divisors ≥ 19). */
        size_t count = 0;
        for (size_t i = 0; i < cur; ++i) {
            if (divs[i] >= 19) ++count;
        }
        cache_len = count;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) {
            free(divs);
            return 0;            /* allocation failure */
        }
        size_t idx = 0;
        for (size_t i = 0; i < cur; ++i) {
            if (divs[i] >= 19) {
                cache[idx++] = (seq_t)((divs[i] - 19) / 2);
            }
        }
        free(divs);

        /* Sort the cache in ascending order. */
        qsort(cache, cache_len, sizeof(seq_t), cmp_seq);
    }

    if (n == 0) return 0;                            /* guard */
    if ((idx_t)(n - 1) < cache_len) {
        return cache[n - 1];
    } else {
        return 0;                                    /* out of range */
    }
}
