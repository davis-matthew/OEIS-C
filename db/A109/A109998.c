/*
 * A109998  Non-Cunningham primes: primes isolated from any Cunningham chain
 *          under any iteration of 2p+-1 or (p+-1)/2.
 * https://oeis.org/A109998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth prime p such that 2p-1, 2p+1, (p-1)/2 and (p+1)/2
 *            are all composite
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Deterministic Miller–Rabin for 64‑bit integers */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((__int128)a * b % m);
}

static inline uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t res = 1;
    while (d) {
        if (d & 1) res = mul_mod(res, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* bases sufficient for deterministic test up to 2^64 */
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_cnt = 0;
    static uint64_t cand = 3;           /* first odd candidate */

    while (cache_cnt < n) {
        /* skip non‑primes */
        if (!is_prime_uint64(cand)) {
            cand += 2;
            continue;
        }

        uint64_t h1 = (cand - 1) >> 1;      /* (p-1)/2 */
        uint64_t h2 = h1 + 1;               /* (p+1)/2 */
        uint64_t two = cand << 1;           /* 2p */

        /* If any of the related numbers is prime, skip */
        if (is_prime_uint64(two - 1) ||
            is_prime_uint64(two + 1) ||
            is_prime_uint64(h1)     ||
            is_prime_uint64(h2)) {
            cand += 2;
            continue;
        }

        /* add to cache */
        if (cache_cnt == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_cnt++] = (seq_t)cand;

        cand += 2;   /* next odd candidate */
    }

    return cache[n - 1];
}
