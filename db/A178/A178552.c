/*
 * A178552  Primes with exactly three 3's.
 * https://oeis.org/A178552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 135, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit3_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A178552: Primes with exactly three 3's.
 * Implementation uses incremental generation, digit counting and deterministic
 * Miller–Rabin primality test for 64‑bit values.  Results are cached
 * to allow arbitrary, out‑of‑order calls.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit3_count_u64.h"

/* ---------- Helper: modular arithmetic for 64‑bit numbers ---------- */
static inline uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

static inline uint64_t mod_pow_u64(uint64_t a, uint64_t d, uint64_t m)
{
    uint64_t r = 1;
    while (d)
    {
        if (d & 1)
            r = mod_mul_u64(r, a, m);
        a = mod_mul_u64(a, a, m);
        d >>= 1;
    }
    return r;
}

/* ---------- Deterministic Miller–Rabin for 64‑bit integers ---------- */
static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;

    /* Small prime trial division */
    static const uint64_t small[] =
        {2ull,3ull,5ull,7ull,11ull,13ull,17ull,19ull,23ull,29ull,31ull,37ull};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        uint64_t p = small[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* Write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] =
        {2ull, 325ull, 9375ull, 28178ull, 450775ull, 9780504ull, 1795265022ull};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ---------- Sequence generator ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)          /* out of range */
        return 0;

    /* Static cache – retains results between calls */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static uint64_t next_candidate = 2333;   /* smallest value with 3 '3's */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        uint64_t cand = next_candidate;

        /* Skip obvious non‑prime endings (even or 5) */
        uint64_t last = cand % 10;
        if (!(last == 1 || last == 3 || last == 7 || last == 9))
        {
            ++next_candidate;
            continue;
        }

        if (digit3_count_u64(cand) == 3 && is_prime_u64(cand))
        {
            /* Grow cache if necessary */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }

        ++next_candidate;
    }

    return cache[n - 1];   /* OEIS offset starts at 1 */
}
