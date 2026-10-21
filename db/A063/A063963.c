/*
 * A063963  Numbers k such that k and k+2540 have the same number of divisors.
 * https://oeis.org/A063963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) is a number k such that d(k)=d(k+2540) (where d(x) is the
 *            number of divisors of x). The function generates the sequence by
 *            a sieve that stores divisor counts up to the current limit, then
 *            scans for equal counts at distance 2540. The search limit is
 *            doubled until at least n terms have been id...
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define OFFSET 2540U   /* difference between k and k+2540 */

/* static storage for memoised output terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* current number of cached terms */
static size_t cache_cap = 0;   /* capacity of cache array */

/* build the cache up to new_limit (maximum k to test) */
static void extend_range(uint32_t new_limit)
{
    const uint32_t size = new_limit + OFFSET + 1U;          /* array size needed */
    uint32_t *spf = (uint32_t *)calloc(size, sizeof(uint32_t));
    if (!spf) abort();   /* memory failure */
    uint32_t *primes = (uint32_t *)malloc(size * sizeof(uint32_t));
    if (!primes) abort();
    size_t primes_cnt = 0;

    /* linear sieve to compute smallest prime factor for each number */
    for (uint32_t i = 2; i < size; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod >= size) break;
            spf[(uint32_t)prod] = p;
            if (p == spf[i]) break;
        }
    }

    /* divisor counts using spf */
    uint32_t *d = (uint32_t *)malloc(size * sizeof(uint32_t));
    if (!d) abort();
    d[0] = 0;
    d[1] = 1;
    for (uint32_t i = 2; i < size; ++i) {
        uint32_t p = spf[i];
        uint32_t m = i / p;
        uint32_t e = 1U;
        while (m % p == 0U) { m /= p; ++e; }
        d[i] = (e + 1U) * d[m];
    }

    /* rebuild cache with all k <= new_limit satisfying the condition */
    free(cache);            /* discard old cache */
    cache_len = 0;
    cache_cap = new_limit + 32U; /* initial capacity */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) abort();

    for (uint32_t k = 1; k <= new_limit; ++k) {
        if (d[k] == d[k + OFFSET]) {
            if (cache_len == cache_cap) {
                cache_cap *= 2U;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) abort();
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    free(spf);
    free(d);
    free(primes);
}

/* ensure the cache contains at least n terms */
static void ensure_terms(idx_t n)
{
    static uint32_t current_limit = 0;
    while (cache_len < (size_t)n) {
        uint32_t new_limit = current_limit == 0 ? 2000U : current_limit * 2U;
        extend_range(new_limit);
        current_limit = new_limit;
    }
}

seq_t a(idx_t n)
{
    /* a(1) is the first term in the OEIS b-file */
    ensure_terms(n);
    return cache[(size_t)n - 1U];
}
