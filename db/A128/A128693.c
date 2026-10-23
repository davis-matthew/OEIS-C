/*
 * A128693  Numbers of the form 3^k*p, where 1 <= k <= 6 and p is a prime
 *          different from 3.
 * https://oeis.org/A128693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of the form 3^k * p, where 1 ≤ k ≤ 6 and p is a prime ≠ 3
   (sorted increasingly). */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* Comparator for qsort when compiling as C. */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *xa = (const seq_t *)a;
    const seq_t *xb = (const seq_t *)b;
    if (*xa < *xb) return -1;
    if (*xa > *xb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defensive, not needed */

    /* Static cache for previously computed terms */
#ifdef __cplusplus
    static std::vector<seq_t> cache_vec;
    static size_t cache_len = 0;
#else
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
#endif

    size_t need = (size_t)n;
    if (need > cache_len) {
        size_t bound = need * 15 + 100;
        if (bound < 100) bound = 100;

        while (1) {
            size_t prime_cnt;
            uint64_t *primes = NULL;
            build_primes_upto((idx_t)bound, &primes, &prime_cnt);
            if (!primes) return (seq_t)0;

#ifdef __cplusplus
            std::vector<seq_t> values;
            values.reserve(prime_cnt * 6);
#else
            size_t values_cap = prime_cnt * 6;
            seq_t *values = (seq_t *)malloc(values_cap * sizeof(seq_t));
            if (!values) {
                free(primes);
                return (seq_t)0;
            }
            size_t values_len = 0;
#endif

            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = primes[i];
                if (p == 3ULL) continue;           /* skip 3 */

                seq_t val = (seq_t)p;
                for (int k = 1; k <= 6; ++k) {
                    val *= (seq_t)3;
#ifdef __cplusplus
                    values.push_back(val);
#else
                    if (values_len < values_cap) {
                        values[values_len++] = val;
                    }
#endif
                }
            }
            free(primes);

#ifdef __cplusplus
            if (values.size() < need) {
                /* Need more primes – enlarge bound and retry. */
                bound *= 2;
                continue;
            }
#else
            if (values_len < need) {
                /* Need more primes – enlarge bound and retry. */
                bound *= 2;
                free(values);
                continue;
            }
#endif

            /* Sort the generated values. */
#ifdef __cplusplus
            std::sort(values.begin(), values.end(), [](const seq_t &a, const seq_t &b) { return a < b; });
#else
            qsort(values, (size_t) (values_len ? values_len : values_cap),
                  sizeof(seq_t), cmp_seq);
#endif

            /* Store the first `need` numbers in the cache. */
#ifdef __cplusplus
            cache_vec.assign(values.begin(), values.begin() + need);
            cache_len = need;
#else
            seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
            if (!new_cache) {
                free(values);
                return (seq_t)0;
            }
            for (size_t i = 0; i < need; ++i) {
                new_cache[i] = values[i];
            }
            free(values);
            cache = new_cache;
            cache_len = need;
#endif
            /* Ready to answer queries. */
            break;
        }
    }

    /* Return the requested term. */
#ifdef __cplusplus
    return cache_vec[need - 1];
#else
    return cache[need - 1];
#endif
}
