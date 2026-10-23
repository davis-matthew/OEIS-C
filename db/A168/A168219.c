/*
 * A168219  Naturals n for which 1 + 10*n^3 (A168147) is prime.
 * https://oeis.org/A168219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 1139, u32 n <= 1139, u64 n <= 1139,
 *            u128 n <= 1139, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A168219: a(n) = the n-th natural number k such that 1 + 10*k^3 is prime.
   Candidates are tested in increasing order, skipping k ≡ 2 (mod 3).
   Primality is tested with a deterministic Miller–Rabin routine valid for 64‑bit
   numbers, which suffices for n ≤ 10000 (1 + 10*n^3 < 2^64).
*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- 64‑bit deterministic Miller–Rabin primality test ---------- */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d)
    {
        if (d & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return r;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
        31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i)
    {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- Cache for found terms ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

static void ensure_capacity(void)
{
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    else if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap << 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined, safe return */

    while (cache_len < n)
    {
        idx_t cand = next_candidate++;
        if ((cand % 3) == 2) continue;   /* 3k+2 cannot yield a prime */

        uint64_t val = 1ULL + 10ULL * (uint64_t)cand * (uint64_t)cand * (uint64_t)cand;
        if (is_prime_u64(val))
        {
            ensure_capacity();
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
