/*
 * A121047  a(0) = a(1) = 1; for n >= 2, a(n) = the n-th integer from among
 *          those positive integers which are coprime to a(n-1)+a(n-2).
 * https://oeis.org/A121047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 64 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A121047: a(0)=a(1)=1;
   for n≥2, a(n) is the n-th positive integer coprime to a(n‑1)+a(n‑2).  
   The implementation caches computed terms and finds each n‑th coprime by
   binary search using inclusion–exclusion on the prime factors of the sum. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* count numbers ≤ x that are coprime to the product of primes[0..k-1] */
static uint64_t count_coprime_to(uint64_t x, uint64_t *primes, size_t k)
{
    uint64_t cnt = x;
    uint64_t subsets = 1ULL << k;
    for (uint64_t mask = 1; mask < subsets; ++mask) {
        uint64_t prod = 1ULL;
        int bits = __builtin_popcountll(mask);
        for (size_t j = 0; j < k; ++j) {
            if (mask & (1ULL << j)) prod *= primes[j];
        }
        uint64_t term = x / prod;
        if (bits & 1) cnt -= term;
        else          cnt += term;
    }
    return cnt;
}

/* smallest prime factor – simple trial division, works for 64‑bit numbers */
static uint64_t spf_uint64(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL) {
        if (n % d == 0ULL) return d;
    }
    return n;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (cache_cap <= (size_t)n) {
        size_t old_cap = cache_cap;
        cache_cap = (size_t)n + 1;
        cache = mem_resize(seq_t, cache, cache_cap);
        for (size_t i = old_cap; i <= (size_t)n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = (seq_t)1;
                continue;
            }

            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            uint64_t S = (uint64_t)(prev1 + prev2);   /* assume fits in 64‑bit */

            /* factor S */
            uint64_t primes[64];
            size_t pf_cnt = 0;
            uint64_t tmp = S;
            if (tmp > 1) {
                while (tmp > 1 && pf_cnt < 64) {
                    uint64_t p = spf_uint64(tmp);
                    primes[pf_cnt++] = p;
                    while (tmp % p == 0) tmp /= p;
                }
            }

            /* binary search for the n‑th coprime to S */
            uint64_t target = (uint64_t)n;
            uint64_t low = 1;
            uint64_t high = (uint64_t)((unsigned __int128)target * S);
            if (high == 0) high = 1;              /* guard when n=0 (not used) */
            while (count_coprime_to(high, primes, pf_cnt) < target) {
                if (high == UINT64_MAX) break;    /* cannot grow further */
                high = high * 2;
                if (high == 0) high = UINT64_MAX;  /* overflow guard */
            }
            while (low < high) {
                uint64_t mid = low + (high - low) / 2;
                if (count_coprime_to(mid, primes, pf_cnt) >= target)
                    high = mid;
                else
                    low = mid + 1;
            }
            cache[i] = (seq_t)low;
        }
    }
    return cache[n];
}
