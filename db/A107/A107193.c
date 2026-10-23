/*
 * A107193  Primes of the form x^2 + 81y^2.
 * https://oeis.org/A107193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 528, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the primes of the form x^2 + 81*y^2 (OEIS A107193).
   The routine generates primes consecutively, tests whether a prime
   satisfies the quadratic representation, and caches the results.
   Subsequent calls for larger indices reuse the cached primes. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* 64‑bit integer square root (floor) – no floating‑point ops   */
static uint64_t isqrt64(uint64_t n)
{
    uint64_t low = 0, high = n, mid, res = 0;
    while (low <= high)
    {
        mid = (low + high) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
        {
            res = mid;
            low = mid + 1;
        }
        else
            high = mid - 1;
    }
    return res;
}

/*  ------------------------------------------------------------ */
/*  Modular exponentiation (a^e mod m) for 64‑bit values          */
static uint64_t modpow_uint64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t result = 1 % m;
    uint64_t base   = a % m;
    while (e)
    {
        if (e & 1U)
            result = (uint64_t)((__uint128_t)result * base % m);
        base = (uint64_t)((__uint128_t)base * base % m);
        e >>= 1U;
    }
    return result;
}

/*  ------------------------------------------------------------ */
/*  Deterministic Miller–Rabin primality test for 64‑bit n       */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2U) return false;

    /* small primes */
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
        19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i)
    {
        if (n % small[i] == 0U) return n == small[i];
    }

    /* write n-1 = d * 2^r with d odd */
    uint64_t d = n - 1;
    unsigned r = 0U;
    while ((d & 1U) == 0U)
    {
        d >>= 1U;
        ++r;
    }

    /* witnesses for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0U) continue;
        uint64_t x = modpow_uint64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        for (unsigned j = 1U; j < r; ++j)
        {
            x = (uint64_t)((__uint128_t)x * x % n);
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

/*  ------------------------------------------------------------ */
/*  Pre‑computed table of quadratic residues modulo 81          */
static bool mod81_sq[81];
static bool mod81_inited = false;

static void init_mod81(void)
{
    for (int i = 0; i < 81; ++i) mod81_sq[i] = false;
    for (int a = 0; a < 81; ++a)
        mod81_sq[(a * a) % 81] = true;
    mod81_inited = true;
}

/*  ------------------------------------------------------------ */
/*  Tests whether a prime p can be written as x^2 + 81*y^2          */
static bool representable_uint64(uint64_t p)
{
    if (!mod81_inited) init_mod81();

    if (p < 97ULL)               /* 97 is the first such prime */
        return false;
    if (!mod81_sq[p % 81U])      /* quick residue test */
        return false;

    uint64_t y_max = isqrt64(p / 81ULL);
    for (uint64_t y = 0ULL; y <= y_max; ++y)
    {
        uint64_t t = p - 81ULL * y * y;
        uint64_t x = isqrt64(t);
        if (x * x == t) return true;
    }
    return false;
}

/*  ------------------------------------------------------------ */
/*  Main sequence generator                                       */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t  cache_sz  = 0;
    static size_t  cache_cap = 0;
    static uint64_t current  = 2;          /* next integer to test */

    while (cache_sz < (size_t)n)
    {
        /* find next representable prime */
        while (true)
        {
            if (is_prime_uint64(current) && representable_uint64(current))
            {
                if (cache_sz >= cache_cap)
                {
                    cache_cap = cache_cap ? cache_cap << 1 : 4;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_sz++] = (seq_t)current;
                ++current;                 /* avoid repetition */
                break;
            }
            ++current;
        }
    }
    return cache[n-1];
}
