/*
 * A093514  Transform of the prime sequence by the Rule90 cellular automaton.
 * https://oeis.org/A093514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th smallest integer m satisfying (isPrime(m) XOR
 *            isPrime(m-2)) . The sequence is the sorted list of such
 *            integers. In particular, n is in the set if either n or n‑2 is
 *            prime but not both
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------- Miller–Rabin primality test (deterministic for 64‑bit) --------- */
#if defined(__SIZEOF_INT128__)
typedef unsigned __int128 u128;
#else
#error "This implementation requires 128‑bit integer support."
#endif

static uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod) {
    return (u128)a * b % mod;
}

static uint64_t modpow64(uint64_t a, uint64_t d, uint64_t mod) {
    uint64_t res = 1;
    while (d) {
        if (d & 1) res = modmul64(res, a, mod);
        a = modmul64(a, a, mod);
        d >>= 1;
    }
    return res;
}

static bool isPrime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* Deterministic bases for n < 2^64 (suffices for our usage) */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* --------- Sequence generation and caching --------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 1;  /* last integer m processed */

/* Ensure cache has space for at least 'need' terms */
static void ensure_cache(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Generate terms until we have at least 'n' terms */
static void generate_terms(idx_t n) {
    if (n <= cache_len) return;
    ensure_cache(n);
    for (;;) {
        ++last_checked;
        uint64_t m = last_checked;
        bool inc = (isPrime_uint64(m) != isPrime_uint64(m >= 2 ? m - 2 : 0));
        if (inc) {
            cache[cache_len++] = (seq_t)m;
            if (cache_len >= (size_t)n) break;
        }
    }
}

/* --------- User function --------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* offset 1, but guard for 0 */
    generate_terms((idx_t)n);
    return cache[n - 1];
}
