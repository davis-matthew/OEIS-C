/*
 * A154669  Averages k of twin prime pairs such that 2*k^3 + 12*k^2 is a
 *          square.
 * https://oeis.org/A154669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154669: averages of twin prime pairs (p,p+2) with 2*k^3+12*k^2 a square.
   k = 2*(t^2-3) where t ≥ 3 and both 2*t^2-7 and 2*t^2-5 are prime.  */

/* Minimal headers */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- 64‑bit deterministic Miller‑Rabin --------------------------------- */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            res = mul_mod_uint64(res, b, mod);
        b = mul_mod_uint64(b, b, mod);
        exp >>= 1ULL;
    }
    return res;
}

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* trial division by small primes */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        if (n == small[i]) return true;
        if (n % small[i] == 0ULL) return false;
    }

    /* Miller‑Rabin with deterministic bases for 64‑bit integers */
    uint64_t d = n - 1;
    int r = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1ULL;
        ++r;
    }

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;

        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1ULL || x == n - 1ULL) continue;

        bool composite = true;
        for (int j = 1; j < r; ++j)
        {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1ULL)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ----------  Sequence generator --------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t  *cache   = NULL;
    static size_t  cache_sz = 0;
    static size_t  cache_cap = 0;
    static idx_t   next_t = 3;  /* t starts from 3 (gives first term k = 12) */

    if (n == 0) return 0;   /* undefined for offset 1, but harmless */

    /* initialise cache lazily */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_sz = 0;
    }

    while (cache_sz < n)
    {
        uint64_t t   = (uint64_t)next_t;
        uint64_t val = 2ULL * t * t;       /* 2*t^2 */
        uint64_t a1  = val - 7ULL;          /* 2*t^2-7 */
        uint64_t a2  = val - 5ULL;          /* 2*t^2-5 */

        if (is_prime_uint64(a1) && is_prime_uint64(a2))
        {
            seq_t k = (seq_t)(val - 6ULL); /* 2*(t^2-3) */
            if (cache_sz == cache_cap)
            {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_sz++] = k;
        }
        ++next_t;
    }

    return cache[n - 1];
}
