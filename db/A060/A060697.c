/*
 * A060697  The sum of the first a(n) composite numbers plus 1 is a prime.
 * https://oeis.org/A060697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A060697:  a(n) = the index of the nth composite number such that
    the sum of the first a(n) composites plus 1 is a prime.  The
    function incrementally builds this sequence, storing results in a
    static cache so that calls may be in any order.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/mod_mul.h"

/* modular multiplication for 64‑bit integers */

/* modular exponentiation for 64‑bit integers */
static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t r = 1;
    while (exp) {
        if (exp & 1) r = mod_mul(r, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return r;
}

/* deterministic Miller–Rabin for 64‑bit integers */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    uint64_t d = n-1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        { 2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL };
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* The required function */
seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_n = 0;            /* largest n already computed */
    static uint64_t next_val = 4;       /* next integer to test for compositeness */
    static unsigned __int128 sum = 0;   /* sum of composites processed so far */
    static idx_t comp_cnt = 0;          /* number of composites processed */

    /* initialise cache */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* grow cache if required */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    while (last_n < n) {
        /* find next composite and test whether sum+1 is prime */
        while (true) {
            /* skip primes */
            while (is_prime_u64(next_val)) ++next_val;

            /* next_val is composite */
            ++comp_cnt;
            sum += (unsigned __int128)next_val;
            ++next_val;  /* advance to the following integer */

            /* candidate prime is sum + 1   (assumes it fits in 64‑bit) */
            if (sum <= (unsigned __int128)UINT64_MAX) {
                uint64_t candidate = (uint64_t)(sum + 1);
                if (is_prime_u64(candidate)) {
                    cache[++last_n] = (seq_t)comp_cnt;
                    break;            /* ready for next n */
                }
            }
        }
    }

    return cache[n];
}
