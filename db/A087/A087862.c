/*
 * A087862  Primes of the form 16*m^2 + 169, m=1,2,3,...
 * https://oeis.org/A087862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime of the form 16*m*m + 169, m = 1,2,3,…
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 2409, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 51 terms through n = 7303 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
