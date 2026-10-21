/*
 * A091992  Numbers n such that 5*n-3 and 5*n+3 are both primes.
 * https://oeis.org/A091992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 2191, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A091992: a(n) = n such that 5*n-3 and 5*n+3 are both prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Primitive modular arithmetic ---------- */
static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d)
    {
        if (d & 1)
            result = mul_mod(result, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return result;
}

/* ---------- Deterministic Miller–Rabin primality test for 64‑bit numbers ---------- */
static bool is_prime64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small primes */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n % small_primes[i] == 0)
            return n == small_primes[i];
    }

    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers (https://miller-rabin.appspot.com/) */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ---------- Caching of found terms ---------- */
static uint64_t *cache_u64 = NULL;   /* stores the raw n values */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;     /* next n to test */

/* enlarge cache if needed */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    uint64_t *new_cache = (uint64_t*)realloc(cache_u64, new_cap * sizeof(uint64_t));
    if (!new_cache)
        abort();
    cache_u64 = new_cache;
    cache_cap = new_cap;
}

/* find the next n such that 5*n-3 and 5*n+3 are both prime and store it */
static void find_next(void)
{
    while (1)
    {
        ++next_candidate;
        uint64_t p1 = 5 * next_candidate - 3;
        uint64_t p2 = 5 * next_candidate + 3;
        if (is_prime64(p1) && is_prime64(p2))
        {
            ensure_cache_cap(cache_len + 1);
            cache_u64[cache_len++] = next_candidate;
            return;
        }
    }
}

/* ---------- Public API required by the build scripts ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset is 1, but we handle 0 gracefully */
    while (cache_len < n)
        find_next();                    /* grow the cache up to the requested index */
    return (seq_t)cache_u64[n - 1];
}
