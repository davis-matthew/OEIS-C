/*
 * A087862  Primes of the form 16*m^2 + 169, m=1,2,3,...
 * https://oeis.org/A087862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime of the form 16*m*m + 169, m = 1,2,3,…
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 21, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mod_mul64(result, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_seq(seq_t n) {
    /* Quick test for 64‑bit sequences – deterministic Miller‑Rabin */
#if defined(__SIZEOF_INT128__) || defined(_MSC_VER)
    if (sizeof(seq_t) == 8) {
        uint64_t x = (uint64_t)n;
        if (x < 2) return false;
        static const uint8_t smalls[] = {2,3,5,7,11,13,17,19,23,29,31,37};
        for (size_t i = 0; i < sizeof(smalls); ++i) {
            if (x == smalls[i]) return true;
            if (x % smalls[i] == 0) return false;
        }
        uint64_t d = x - 1, s = 0;
        while ((d & 1) == 0) { d >>= 1; ++s; }
        static const uint64_t bases[] = {
            2ULL, 325ULL, 9375ULL, 28178ULL,
            450775ULL, 9780504ULL, 1795265022ULL
        };
        for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
            uint64_t a = bases[i] % x;
            if (a == 0) continue;
            uint64_t y = mod_pow64(a, d, x);
            if (y == 1 || y == x - 1) continue;
            bool composite = true;
            for (uint64_t r = 1; r < s; ++r) {
                y = mod_mul64(y, y, x);
                if (y == x - 1) { composite = false; break; }
            }
            if (composite) return false;
        }
        return true;
    }
#endif
    /* Fallback for arbitrary‑precision seq_t */
    return is_prime_big(n);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t m = 1;          /* index used in 16*m*m+169 */

    if (n < 1)
        return 0;                /* offset is 1 */

    /* Allocate cache if first call */
    if (!cache) {
        cache_cap = 8;
        cache = seq_alloc(cache_cap);
    }

    while (cache_size < (size_t)n) {
        seq_t candidate = (seq_t)16 * (seq_t)m * (seq_t)m + (seq_t)169;

        if (is_prime_seq(candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort(); /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
        ++m;
    }

    return cache[n - 1];
}
