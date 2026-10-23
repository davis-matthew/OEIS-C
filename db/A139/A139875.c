/*
 * A139875  Primes of the form 4x^2 + 4xy + 43y^2.
 * https://oeis.org/A139875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 411, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A139875
 *  Primes of the form 4*x^2 + 4*x*y + 43*y^2.
 *  All such primes are congruent to 43, 67 or 163 modulo 168.
 *  This implementation generates primes with those residues using a
 *  deterministic Miller–Rabin test (sufficient for 64‑bit values).
 *  The results are cached and served in order, so each call to a(n)
 *  is O(1) after initial generation.  No external utilities are used. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = mul_mod64(x, x, n);
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

static seq_t *prime_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;   /* next number to test */

/* Ensure that at least n primes are cached */
static void ensure_terms(idx_t n)
{
    if (cache_len >= n) return;

    if (!prime_cache)
    {
        cache_cap = 32;
        prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    while (cache_len < n)
    {
        /* Advance to next number that satisfies the residue condition */
        while ( !( (next_candidate % 168 == 43) ||
                   (next_candidate % 168 == 67) ||
                   (next_candidate % 168 == 163) ) )
        {
            ++next_candidate;
        }

        if (is_prime_u64(next_candidate))
        {
            if (cache_len + 1 > cache_cap)
            {
                cache_cap = cache_cap * 2;
                prime_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
            }
            prime_cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 */
    ensure_terms(n);
    return prime_cache[n - 1];
}
