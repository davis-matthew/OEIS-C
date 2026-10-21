/*
 * A064978  n*10^4-1, n*10^4-3, n*10^4-7 and n*10^4-9 are all prime.
 * https://oeis.org/A064978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A064978 – the n‑th integer k such that
   10000*k-1, 10000*k-3, 10000*k-7 and 10000*k-9 are all prime.
   Implementation uses deterministic Miller–Rabin primality testing
   for 64‑bit integers and dynamic caching of found terms. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Miller–Rabin for 64‑bit integers (deterministic for n < 3.4e12) */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t r = 1;
    while (d) {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool continue_outer = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) { continue_outer = true; break; }
        }
        if (continue_outer) continue;
        return false;
    }
    return true;
}

/* cache for sequence values */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

static inline bool candidate_ok(idx_t k) {
    uint64_t base = k * 10000ULL;
    static const uint32_t offs[4] = {1,3,7,9};
    for (int i = 0; i < 4; ++i) {
        uint64_t x = base - offs[i];
        if (!is_prime_uint64(x)) return false;
    }
    return true;
}

static inline void ensure_capacity(void) {
    if (cache_len == cache_cap) {
        size_t newcap = cache_cap == 0 ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t*)realloc(seq_cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        cache_cap = newcap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but caller should not request 0 */
    while (cache_len < n) {
        if (candidate_ok(next_candidate)) {
            ensure_capacity();
            seq_cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
