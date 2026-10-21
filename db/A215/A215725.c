/*
 * A215725  Numbers x such that 2*x+1 is prime as is 2*x+3 and x^2+x+41 and
 *          (x+1)^2+(x+1)+41.
 * https://oeis.org/A215725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 188, u32 n <= 1300, u64 n <= 1300,
 *            u128 n <= 1300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes sequence A215725 by searching for integers x such that
   2*x+1, 2*x+3, x^2+x+41 and (x+1)^2+(x+1)+41 are all prime.  The result is
   memoised in a static dynamic array, so that repeated calls for any n ≥ 1
   use previously found terms.  The implementation uses a deterministic
   Miller–Rabin test for 64‑bit numbers.  The code is valid C11 and C++17. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----- Miller–Rabin primality test for 64‑bit numbers ----- */
static uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1U) result = mulmod_uint64(result, base, mod);
        base = mulmod_uint64(base, base, mod);
        exp >>= 1U;
    }
    return result;
}
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1U) == 0) { d >>= 1U; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = powmod_uint64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mulmod_uint64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----- Cache and sequence generation ----- */
static seq_t *cache = NULL;    /* array of previously found terms */
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* capacity of allocated array */
static idx_t next_x = 0;       /* last tested candidate (0 means none tested) */

static inline void ensure_capacity(size_t needed) {
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* not used; safety */
    while (cache_len < n) {
        idx_t x = ++next_x;                /* candidate x */
        uint64_t p1 = 2 * x + 1;
        if (!is_prime_uint64(p1)) continue;
        uint64_t p2 = 2 * x + 3;
        if (!is_prime_uint64(p2)) continue;
        __int128 v1 = (__int128)x * x + x + 41;
        uint64_t q1 = (uint64_t)v1;
        if (!is_prime_uint64(q1)) continue;
        __int128 v2 = (__int128)(x + 1)*(x + 1) + (x + 1) + 41;
        uint64_t q2 = (uint64_t)v2;
        if (!is_prime_uint64(q2)) continue;
        /* success: record term */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)x;
    }
    return cache[n-1];
}
