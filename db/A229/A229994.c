/*
 * A229994  For every positive integer m, let u(m) = (d(1),d(2),...,d(k)) be
 *          the unitary divisors of m in increasing order. Let Q be the
 *          concatenation of the vectors (d(k)/d(1), d(k-1)/d(2), ...,
 *          d(1)/d(k)), so that every positive rational number appears in Q
 *          exactly once. The numerators form A229994; the de...
 * https://oeis.org/A229994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) enumerates the numerators obtained from the unitary–divisor
 *            ratios that list all positive rationals exactly once
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* Factor a number into distinct prime powers.  The array pp[] must have
   room for at least 20 entries.  The number of distinct prime factors
   is returned in *cnt. */
static void prime_powers(uint64_t n, uint64_t pp[], size_t *cnt)
{
    size_t c = 0;
    uint64_t t = n;
    if (t % 2 == 0) {
        uint64_t p = 1;
        while (t % 2 == 0) { t /= 2; p *= 2; }
        pp[c++] = p;
    }
    for (uint64_t f = 3; f * f <= t; f += 2) {
        if (t % f == 0) {
            uint64_t p = 1;
            while (t % f == 0) { t /= f; p *= f; }
            pp[c++] = p;
        }
    }
    if (t > 1) pp[c++] = t;
    *cnt = c;
}

#ifdef __cplusplus
static std::vector<seq_t> cacheVec;
static idx_t next_m = 1;
static void add_to_cache(seq_t val) { cacheVec.push_back(val); }
#else
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_m = 1;
static void add_to_cache(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}
/* Comparator for qsort (ascending order) */
static int cmp_asc(const void *a, const void *b)
{
    const seq_t *x = (const seq_t*)a;
    const seq_t *y = (const seq_t*)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}
#endif

/* Generate the unitary divisors of m, sort them, then append the
   reversed list (d(k)/d(1), …, d(1)/d(k)) numerators to the cache. */
static void process_m(idx_t m)
{
    uint64_t pp[20];
    size_t cnt;
    prime_powers((uint64_t)m, pp, &cnt);

    size_t total = 1ULL << cnt;          /* 2^cnt  <= 2^20 */
#ifdef __cplusplus
    std::vector<seq_t> divs;
    divs.reserve(total);
    for (size_t mask = 0; mask < total; ++mask) {
        seq_t prod = 1;
        for (size_t i = 0; i < cnt; ++i)
            if (mask & (1ULL << i))
                prod = prod * pp[i];
        divs.push_back(prod);
    }
    std::sort(divs.begin(), divs.end(),
              [](const seq_t& a, const seq_t& b){ return a < b; });
    for (size_t i = total; i > 0; --i)
        add_to_cache(divs[i-1]);
#else
    seq_t *divs = (seq_t*)malloc(total * sizeof(seq_t));
    if (!divs) exit(1);
    for (size_t mask = 0; mask < total; ++mask) {
        seq_t prod = 1;
        for (size_t i = 0; i < cnt; ++i)
            if (mask & (1ULL << i))
                prod = prod * pp[i];
        divs[mask] = prod;
    }
    qsort(divs, total, sizeof(seq_t), cmp_asc);
    for (size_t i = total; i > 0; --i)
        add_to_cache(divs[i-1]);
    free(divs);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safeguard against invalid calls */
#ifdef __cplusplus
    while (cacheVec.size() < n) {
        process_m(next_m);
        ++next_m;
    }
    return cacheVec[n-1];
#else
    while (cache_len < n) {
        process_m(next_m);
        ++next_m;
    }
    return cache[n-1];
#endif
}
