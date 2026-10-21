/*
 * A224864  Numbers n such that 90*n + 77 and 90*n + 79 are twin primes.
 * https://oeis.org/A224864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth integer n such that 90*n+77 and 90*n+79 are
   both prime.  The function generates terms lazily and caches them. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*------------------------------------------------------------*/
/* 64‑bit deterministic Miller–Rabin primality test */
/*------------------------------------------------------------*/
static uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result = mod_mul64(result, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2)
        return false;

    /* small prime check */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    uint64_t s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* witnesses sufficient for 64‑bit numbers */
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses) / sizeof(witnesses[0]); ++i)
    {
        uint64_t a = witnesses[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mod_mul64(x, x, n);
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

/*------------------------------------------------------------*/
/*  Cache management for the sequence */
/*------------------------------------------------------------*/
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t  next_candidate = 1;

static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

static void add_term(seq_t val)
{
    if (cache_size == cache_cap)
        grow_cache();
    cache[cache_size++] = val;
}

/*------------------------------------------------------------*/
/*  Main sequence function */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* safety for the offset 1 */
    while (cache_size < (size_t)n)
    {
        uint64_t cur = (uint64_t)next_candidate++;
        uint64_t p1   = 90ULL * cur + 77ULL;
        uint64_t p2   = p1 + 2ULL;
        if (isPrime64(p1) && isPrime64(p2))
            add_term((seq_t)cur);
    }
    return cache[n - 1];
}
