/*
 * A099192  Numbers k such that the string k235711131719 is prime.
 * https://oeis.org/A099192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find k such that 10^12*k + 235711131719 is prime.
   Uses a memoised search; for 64‑bit seq_t it employs a fast
   deterministic Miller‑Rabin test.  For larger seq_t sizes it
   falls back to the generic big‑int routine is_prime_big. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/is_prime_big.h"

static inline uint64_t mul64_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)(( (unsigned __int128)a * b ) % mod);
}

static inline uint64_t pow64_mod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL) result = mul64_mod(result, b, mod);
        b = mul64_mod(b, b, mod);
        exp >>= 1ULL;
    }
    return result;
}

static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow64_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mul64_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;

    const seq_t P = 235711131719ULL;
    const seq_t POW10_12 = (seq_t)1000000000000ULL;

    while (cache_len < n) {
        if ((next_k % 3 == 1) || (next_k % 7 == 0) || (next_k % 11 == 3)) {
            ++next_k;
            continue;
        }

        bool prime;
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            uint64_t mk = (uint64_t)next_k;
            uint64_t n64 = mk * (uint64_t)POW10_12 + (uint64_t)P;
            prime = is_prime_uint64(n64);
        } else {
            seq_t N = (seq_t)next_k * POW10_12 + P;
            prime = is_prime_big(N);
        }

        if (prime) {
            if (cache_len == cache_cap) {
                ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache,
                                sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
