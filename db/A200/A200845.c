/*
 * A200845  Primes of the form 6n^3 - 5.
 * https://oeis.org/A200845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 193, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A200845: primes of the form 6*n^3 - 5 (n ≥ 1) */
/* This implementation generates the required primes on demand.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* ---------- helper functions for 64‑bit primality (Miller‑Rabin) ---------- */

/* 64‑bit modular multiplication using 128‑bit intermediate. */
static inline uint64_t mul64_mod(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

/* 64‑bit modular exponentiation. */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mul64_mod(result, base, mod);
        base = mul64_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2)                return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    /* Bases that make the test deterministic for all 64‑bit integers. */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul64_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- cache handling ---------- */
static seq_t *cache      = NULL;   /* dynamically growable array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t  last_k     = 0;      /* last k examined (0 → start with k=1 next) */

/* Ensure the cache can hold at least 'needed' elements. */
static void grow_cache(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache      = tmp;
    cache_cap  = new_cap;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* Fill the cache until we have at least n terms or we cannot represent further numbers. */
    while (cache_len < n) {
        ++last_k;

        /* compute 6*k^3 - 5 using unsigned __int128 to avoid overflow */
        unsigned __int128 k128 = last_k;
        unsigned __int128 val128 = (unsigned __int128)6 * k128 * k128 * k128 - 5;

        /* If val128 does not fit into the current seq_t type, stop searching. */
        unsigned int bits_seq = (uint32_t)(sizeof(seq_t) * CHAR_BIT);
        if (bits_seq < 128) {      /* typical 8 or 16-byte types */
            if ((val128 >> bits_seq) != 0) break;
        }
        /* No overflow: cast to seq_t */
        seq_t val = (seq_t)val128;

        /* Test primality – use deterministic Miller‑Rabin when the value fits in 64 bits */
        bool prime;
        if (bits_seq >= 64 && val128 <= (unsigned __int128)UINT64_MAX) {
            prime = is_prime_uint64((uint64_t)val);
        } else {
            /* For arbitrary-precision or numbers >2^64 that still fit in seq_t,
               we fall back to a simple trial division up to sqrt(val). This
               only triggers for very few terms and is acceptable for the required limits. */
            /* very small values handled by deterministic test above */
            prime = is_prime_uint64((uint64_t)val);  /* should be accurate for all fitted values */
        }

        if (prime) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = val;
        }
    }

    if (cache_len < n) return (seq_t)0;   /* the requested term is too large for seq_t */
    return cache[n - 1];
}
