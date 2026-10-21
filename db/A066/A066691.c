/*
 * A066691  Value of tau(2n-1) when tau(2n-1) = tau(2n+1).
 * https://oeis.org/A066691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(2k‑1) where k is the n‑th integer for which τ(2k‑1) =
 *            τ(2k+1). The function returns that common divisor‑count
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Prime handling ---------------------------------------------------- */

/* store primes up to the current limit */
static uint32_t *prime_list = NULL;
static size_t    prime_cnt  = 0;
static uint32_t  prime_limit = 2;

/* integer square root floor of a 64‑bit unsigned integer */
static uint32_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = 4294967295ULL, mid, res = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq <= n) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return (uint32_t)res;
}

/* Ensure the list of primes contains all primes ≤ limit. */
static void ensure_primes_to(uint32_t limit)
{
    if (limit <= prime_limit) return;

    size_t size = (size_t)limit + 1;
    bool *is_comp = (bool *)calloc(size, sizeof(bool));
    if (!is_comp) abort();

    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;

    size_t new_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++new_cnt;

    uint32_t *new_pr = (uint32_t *)malloc(new_cnt * sizeof(uint32_t));
    if (!new_pr) abort();

    size_t idx = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) new_pr[idx++] = i;

    free(prime_list);
    free(is_comp);

    prime_list  = new_pr;
    prime_cnt   = new_cnt;
    prime_limit = limit;
}

/* --- Divisor count ----------------------------------------------------- */

/* τ(n) for an unsigned 64‑bit integer. */
static uint64_t tau_uint64(uint64_t n)
{
    if (n <= 1) return 1;

    uint64_t result = 1;
    uint64_t tmp     = n;
    uint32_t limit   = isqrt_uint64(tmp);
    ensure_primes_to(limit);

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            uint32_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            result *= (uint64_t)(exp + 1);
        }
    }
    if (tmp > 1) result *= 2;
    return result;
}

/* --- Sequence cache --------------------------------------------------- */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Append a new value to the cache, expanding if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* --- Main sequence function -------------------------------------------- */

/* next odd integer to test (starts at 1). */
static uint64_t next_odd = 1;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety for out‑of‑bounds */

    /* generate terms until we have at least n values */
    while (cache_len < (size_t)n) {
        uint64_t t1 = tau_uint64(next_odd);
        uint64_t t2 = tau_uint64(next_odd + 2);
        if (t1 == t2) cache_append((seq_t)t1);
        next_odd += 2;                      /* move to the next odd integer */
    }
    return cache[(size_t)n - 1];
}
