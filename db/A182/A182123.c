/*
 * A182123  Poulet numbers of the form (6k+1)*(24k+1).
 * https://oeis.org/A182123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 222, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Poulet numbers that are of the form (6k+1)*(24k+1) */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Miller–Rabin deterministic test for 64‑bit integers */
static uint64_t mulmod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp)
    {
        if (exp & 1U)
            res = mulmod_uint64(res, base, mod);
        base = mulmod_uint64(base, base, mod);
        exp >>= 1U;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2U) return false;
    if ((n & 1U) == 0U) return n == 2U;
    if (n % 3U == 0U) return n == 3U;

    uint64_t d = n - 1U;
    unsigned s = 0U;
    while ((d & 1U) == 0U) { d >>= 1U; ++s; }

    static const uint64_t bases[] =
        { 2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0U) continue;
        uint64_t x = powmod_uint64(a, d, n);
        if (x == 1ULL || x == n - 1U) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mulmod_uint64(x, x, n);
            if (x == n - 1U) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static bool is_poulet_uint64(uint64_t n)
{
    if (n < 2U) return false;
    if (is_prime_uint64(n)) return false;
    return powmod_uint64(2ULL, n - 1U, n) == 1ULL;
}

/* Cached generator of Poulet numbers (6k+1)(24k+1) */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;   /* array of discovered terms */
    static size_t cache_len = 0;  /* current number of stored terms */
    static size_t cache_cap = 0;  /* current capacity of cache array */
    static idx_t next_k = 1;      /* next k to test (k starts at 1) */

    /* allocate initial cache if needed */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE); /* memory failure */
        cache_len = 0;
    }

    /* extend cache until we have at least n terms */
    while (cache_len < n)
    {
        /* compute (6k+1)*(24k+1) */
        seq_t ak = ((seq_t)6 * (seq_t)next_k) + 1;
        seq_t bk = ((seq_t)24 * (seq_t)next_k) + 1;
        seq_t val = ak * bk;      /* fits within seq_t for all relevant k */

        /* test for Poulet property (only meaningful for 64‑bit values) */
        uint64_t val64 = (uint64_t)val;  /* safe cast for n up to 2^64‑1 */
        if (is_poulet_uint64(val64))
        {
            /* ensure there is room in cache */
            if (cache_len + 1 > cache_cap)
            {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_len++] = val;
        }

        ++next_k;
    }

    return cache[n - 1];
}
