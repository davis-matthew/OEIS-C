/*
 * A102338  Numbers k such that 10k+3 is prime.
 * https://oeis.org/A102338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th nonnegative integer k such that 10*k+3 is a prime.
   The function returns a(k) = k where 10*k+3 is the nth prime of that form. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mul_mod.h"

/* ---------- Prime test for 64‑bit integers (deterministic) ---------- */

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1ULL) result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* trivial small primes */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }

    /* Miller–Rabin deterministic for 64‑bit numbers */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1;
        ++s;
    }
    /* bases set enough for n < 2^64 */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL,
                                     28178ULL, 450775ULL, 9780504ULL,
                                     1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- Sequence generation ---------- */
seq_t a(idx_t n)
{
    /* static storage for computed terms */
    static seq_t *cache   = NULL;  /* array of k values */
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of values stored */
    static uint64_t last_k   = 0;  /* last k considered */

    /* initial allocation */
    if (cache_cap == 0)
    {
        cache_cap = 128;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    }

    /* extend cache until it contains at least n terms */
    while (cache_len < n)
    {
        uint64_t p = 10ULL * last_k + 3ULL;
        if (is_prime_uint64(p))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
                /* if realloc fails we simply abort (undefined for contest) */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_k;
        }
        ++last_k;
    }

    return cache[n - 1];
}
