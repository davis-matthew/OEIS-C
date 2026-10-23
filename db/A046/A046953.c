/*
 * A046953  Numbers k such that 6*k - 1 is composite.
 * https://oeis.org/A046953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n‑th positive integer k such that 6*k-1 is composite. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*-----  64‑bit Miller–Rabin primality test (deterministic for <=2^64)  -----*/
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    while (exp) {
        if (exp & 1) res = mul_mod_uint64(res, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2, 325, 9375, 28178, 450775, 9780504, 1795265022
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/*-----  Dynamic cache for the sequence  -----*/
static seq_t *cache      = NULL;   /* allocated array of values */
static size_t cache_len  = 0;      /* number of values stored */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t  next_k     = 1;      /* next k to test (always > last tested) */

/* Grow the cache to at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;

    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();           /* memory allocation failure */

    /* copy old data */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

    /* free old array */
    if (cache) free(cache);

    cache      = new_cache;
    cache_cap  = new_cap;
}

/*--------------------------------------------------------------------*/
/* Main sequence function: returns the n-th k with 6k-1 composite. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* n < 1 is undefined; return 0 as placeholder */

    /* generate terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        uint64_t candidate = 6ULL * (uint64_t)next_k - 1ULL;
        if (!is_prime_uint64(candidate)) {
            /* composite candidate – store its k */
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n-1];
}
