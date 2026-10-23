/*
 * A048751  Composites k whose product of divisors divided by number of
 *          divisors is an integer.
 * https://oeis.org/A048751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th composite integer k such that the product of
 *            divisors of k divided by the number of divisors is an integer.
 *            Equivalently: k^(τ(k)/2) mod τ(k) == 0, where τ(k) is the
 *            divisor count of k
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root (floor) using binary search and 128-bit multiplication */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = (n >> 1) + 1;
    while (low <= high)
    {
        uint64_t mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* floor(sqrt(n)) */
}

/* modular exponentiation: base^exp mod mod */
static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0)
    {
        if (exp & 1)
            result = (__uint128_t)result * b % mod;
        b = (__uint128_t)b * b % mod;
        exp >>= 1;
    }
    return result;
}

/* divisor count τ(n) */
static inline uint64_t divisor_count_uint64(uint64_t n)
{
    uint64_t res = 1;
    uint64_t v = n;
    for (uint64_t p = 2; p * p <= v; ++p)
    {
        if (v % p == 0)
        {
            uint64_t e = 0;
            while (v % p == 0)
            {
                v /= p;
                ++e;
            }
            res *= (e + 1);
        }
    }
    if (v > 1)
        res *= 2;
    return res;
}

/* static cache for generated terms */
static seq_t *cache   = NULL; /* array of terms */
static size_t cache_len = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* allocated capacity */
static uint64_t next_candidate = 4; /* next integer to test */

/* ensure enough terms are precomputed */
static void ensure_terms(size_t needed)
{
    while (cache_len < needed)
    {
        uint64_t k = next_candidate++;
        uint64_t d = divisor_count_uint64(k);
        if (d == 2) /* prime, skip */
            continue;

        bool ok = false;
        if (d & 1)        /* d odd -> k must be a square */
        {
            uint64_t m = isqrt_uint64(k);
            if (m * m == k && mod_pow_uint64(m, d, d) == 0)
                ok = true;
        }
        else /* d even */
        {
            if (mod_pow_uint64(k, d / 2, d) == 0)
                ok = true;
        }

        if (ok)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, safe fallback */

    /* convert idx_t to size_t for indexing */
    size_t target = (size_t)n;
    ensure_terms(target);
    return cache[target - 1];
}
