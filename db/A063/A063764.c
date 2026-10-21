/*
 * A063764  Nonprimes k whose largest prime factor > k^(2/3).
 * https://oeis.org/A063764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063764
 * Nonprimes k whose largest prime factor > k^(2/3)
 * This implementation generates terms on demand, caching them.
 * It uses a simple incremental prime sieve and trial division
 * to obtain the largest prime factor.  No floating‑point
 * operations are performed on seq_t values.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>     /* for sqrt on uint64_t values */

/* static cache for sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* static dynamic array of primes */
static uint64_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* last number checked when generating the sequence */
static uint64_t cur_checked = 1;

/* enlarge prime array capacity */
static void prime_grow(void) {
    size_t new_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
    uint64_t *new_ptr = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!new_ptr) exit(EXIT_FAILURE);
    primes = new_ptr;
    primes_cap = new_cap;
}

/* ensure that primes array contains all primes ≤ limit */
static void ensure_primes(uint64_t limit) {
    if (primes_len == 0) {          /* first time: start with 2 */
        if (limit < 2) return;
        prime_grow();
        primes[0] = 2;
        primes_len = 1;
    }
    uint64_t cand = (primes_len > 0) ? primes[primes_len - 1] + 1 : 2;
    for (; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_len == primes_cap) prime_grow();
            primes[primes_len++] = cand;
        }
    }
}

/* compute the largest prime factor of a uint64_t number */
static uint64_t largest_prime_factor_uint64(uint64_t n) {
    if (n <= 1) return 1;
    uint64_t temp = n;
    uint64_t max_pf = 1;
    uint64_t sqrt_limit = (uint64_t)(sqrt((long double)temp)) + 1;
    ensure_primes(sqrt_limit);
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            max_pf = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) max_pf = temp;
    return max_pf;
}

/* append a value to the cache */
static void cache_append(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined but harmless */
    while (cache_len < n) {
        ++cur_checked;
        uint64_t lpf = largest_prime_factor_uint64(cur_checked);
        if (lpf == cur_checked) continue;   /* skip primes */
        __uint128_t lpf_cubed = (__uint128_t)lpf * lpf * lpf;
        __uint128_t cur_sq = (__uint128_t)cur_checked * cur_checked;
        if (lpf_cubed > cur_sq) {
            cache_append((seq_t)cur_checked);
        }
    }
    return cache[n - 1];
}
