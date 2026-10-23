/*
 * A174359  Numbers n such that 6n-1, 6n+1, and 6n+5 are prime.
 * https://oeis.org/A174359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th integer such that 6n-1, 6n+1 and 6n+5 are all prime
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Scan candidates, test primality with deterministic Miller–Rabin for 64‑bit integers,
   cache the first n solutions so that calls can be done in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin for 64‑bit integers */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mod_mul64(result, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Cache of found terms */
static seq_t *cache = NULL;
static size_t cache_sz = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 1;  /* candidate to start next search */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, guard against 0 */
    while (cache_sz < n) {
        idx_t m = last_checked;
        for (;;) {
            /* Special case m == 1 satisfies the condition */
            uint64_t six_m = 6ULL * (uint64_t)m;
            uint64_t p1 = six_m - 1;
            uint64_t p2 = six_m + 1;
            uint64_t p3 = six_m + 5;
            if (is_prime_u64(p1) && is_prime_u64(p2) && is_prime_u64(p3)) {
                /* accept m */
                if (cache_sz == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 4;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_sz++] = (seq_t)m;
                last_checked = m + 1;
                break;
            }
            ++m;
        }
    }
    return cache[n - 1];
}
