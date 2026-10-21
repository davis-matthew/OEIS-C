/*
 * A064260  Values of m such that N=(am+1)(bm+1)(cm+1) is a 3-Carmichael
 *          number (A087788), where a,b,c = 1,2,47.
 * https://oeis.org/A064260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th positive integer m (1‑based) such that
 * (m+1), (2m+1) and (47m+1) are all prime.  The sequence is A064260. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- 64‑bit deterministic Miller–Rabin ---------- */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2u)                return false;
    static const uint64_t small[] = {
        2u,3u,5u,7u,11u,13u,17u,19u,23u,29u,31u,37u,41u,43u,47u
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n-1, s = 0;
    while ((d & 1u) == 0u) { d >>= 1; ++s; }
    static const uint64_t bases[] = {
        2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----------  Sequence generator ---------- */
seq_t a(idx_t n)
{
    /* Never called for n = 0 – just guard */
    if (n == 0) return (seq_t)0;

    /* Cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms cached */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure enough space for n terms */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* State that persists across calls */
    static uint64_t next_k = 0;      /* k value for next candidate m = 22+94*k */
    static uint64_t already_found = 0; /* number of terms already stored */

    /* Generate terms until we have at least n in the cache */
    while (already_found < (size_t)n) {
        uint64_t m = 22ULL + 94ULL * next_k;
        ++next_k;                     /* advance k regardless of success */
        if (!is_prime_uint64(m + 1))           continue;
        if (!is_prime_uint64(2ULL * m + 1))    continue;
        if (!is_prime_uint64(47ULL * m + 1))   continue;
        cache[already_found++] = (seq_t)m;     /* store the found m */
    }
    cache_len = already_found;
    return cache[n-1];
}
