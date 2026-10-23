/*
 * A105128  Primes of the form 64n+33.
 * https://oeis.org/A105128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form 64k + 33 (with offset 1)
 * Range:     u8 n <= 1, u16 n <= 202, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* --- minimal deterministic 64‑bit primality test ------------- */
/* ------------------------------------------------------------ */

static uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    /* use 128‑bit arithmetic when available */
    return ((unsigned __int128)a * b) % mod;
}

static uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t base_mod = base % mod;
    while (exp)
    {
        if (exp & 1)
            result = modmul64(result, base_mod, mod);
        base_mod = modmul64(base_mod, base_mod, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small primes for quick rejection */
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i)
    {
        if (n == small[i])
            return true;
        if (n % small[i] == 0)
            return false;
    }

    /* Miller–Rabin deterministic for 64‑bit numbers
       bases: 2, 325, 9375, 28178, 450775, 9780504, 1795265022
       (cover all 64‑bit integers) */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL,
        28178ULL, 450775ULL, 9780504ULL,
        1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue; /* skip 0 mod n */
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = modmul64(x, x, n);
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

/* ------------------------------------------------------------ */
/*                   sequence function a(n)                     */
/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Cache of primes of the form 64k+33 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t count = 0;          /* number of cached primes */
    static uint64_t last_k = 0;      /* largest k examined so far */

    if (n == 0)            /* a(0) is undefined; return 0 */
        return (seq_t)0;

    /* return immediately if already cached */
    if (n <= count)
        return cache[n - 1];

    /* continue searching until we have at least n primes */
    while (count < n)
    {
        ++last_k;                         /* next k in the progression */
        uint64_t cand = 33ULL + 64ULL * last_k;

        if (is_prime_uint64(cand))
        {
            /* grow cache if necessary */
            if (count == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[count++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
