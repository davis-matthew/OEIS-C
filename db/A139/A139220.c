/*
 * A139220  Numbers k such that 41+(k+k^2)/2 = 41+A000217(k) is prime.
 * https://oeis.org/A139220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 41 + (k + k^2)/2 is prime (OEIS A139220) */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* prime test for 64‑bit numbers (deterministic Miller–Rabin) */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* caching infrastructure for the sequence values (k) */
static seq_t *cache = NULL;      /* cache of generated terms a(1), a(2), … */
static size_t cache_size = 0;    /* number of stored terms */
static size_t cache_cap  = 0;    /* allocated capacity */
static idx_t next_k = 0;         /* next k to test for membership */

/* Expand cache to hold at least new_cap elements. */
static void ensure_cache_cap(size_t new_cap)
{
    if (cache_cap >= new_cap) return;
    size_t new_size = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_size < new_cap) new_size <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!p) abort();  /* allocation failure */
    cache = p;
    cache_cap = new_size;
}

/* Fill cache until at least n terms have been stored. */
static void ensure_terms(idx_t n)
{
    if (cache_size >= n) return;
    ensure_cache_cap(n);
    while (cache_size < n) {
        uint64_t k = (uint64_t)next_k;
        unsigned __int128 tri = (unsigned __int128)k * (k + 1) / 2;
        uint64_t val = (uint64_t)(tri + 41ULL);
        if (is_prime_uint64(val)) {
            cache[cache_size++] = (seq_t)k;
        }
        ++next_k;
    }
}

/* Required public function for OEIS evaluation. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for offset 1, but safe */
    ensure_terms(n);
    return cache[n - 1];
}
