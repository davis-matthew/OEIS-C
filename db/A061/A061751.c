/*
 * A061751  Numbers k such that k! is divisible by (k+1)^3.
 * https://oeis.org/A061751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 145, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061751: Numbers k such that k! is divisible by (k+1)^3 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- helper: integer square root for uint64_t ---------- */
static inline uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t lo = 1, hi = n >> 1, ans = 1;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        if (mid <= n / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* ---------- prime sieve (incremental) ---------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static size_t prime_cap  = 0;

static void ensure_primes_upto(uint64_t limit)
{
    if (prime_cnt == 0) {          /* initialise prime list */
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) return;
        prime_cnt = 0;
    }

    uint64_t start = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
    for (uint64_t n = start; n <= limit; ++n) {
        bool prime = true;
        uint64_t r = isqrt_u64(n);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= r; ++i) {
            if (n % prime_list[i] == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap == 0 ? 64 : prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
                if (!tmp) return;
                prime_list = tmp;
            }
            prime_list[prime_cnt++] = n;
        }
    }
}

/* ---------- test whether k satisfies the condition ---------- */
static bool satisfies(idx_t k)
{
    if (k < 1) return false;
    uint64_t m = (uint64_t)(k + 1);
    uint64_t temp = m;
    uint64_t max_p = isqrt_u64(m);
    ensure_primes_upto(max_p);

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= max_p; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p != 0) continue;
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        /* exponent of p in k! using Legendre's formula */
        idx_t v = 0;
        idx_t t = k;
        while (t) { t /= p; v += t; }
        if ((idx_t)(3 * e) > v) return false;
        max_p = isqrt_u64(temp);
    }
    if (temp > 1) {                /* remaining prime factor of m */
        uint64_t p = temp;
        uint64_t e = 1;
        idx_t v = 0;
        idx_t t = k;
        while (t) { t /= p; v += t; }
        if ((idx_t)(3 * e) > v) return false;
    }
    return true;
}

/* ---------- cache of found terms ---------- */
static seq_t *cache  = NULL;
static size_t cache_cnt  = 0;
static size_t cache_cap  = 0;
static idx_t last_checked = 0;

static void append_to_cache(seq_t val)
{
    if (cache_cnt == cache_cap) {
        cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return;
        cache = tmp;
    }
    cache[cache_cnt++] = val;
}

/* ---------- main sequence generator ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined for n = 0, harmless */
    while (cache_cnt < (size_t)n) {
        ++last_checked;
        if (last_checked == 0) continue; /* skip k = 0 */
        if (satisfies(last_checked))
            append_to_cache((seq_t)last_checked);
    }
    return cache[(size_t)n - 1];
}
