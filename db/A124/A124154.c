/*
 * A124154  Numbers n such that 1 + n + n^3 + n^5 is prime.
 * https://oeis.org/A124154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth even integer k such that 1 + k + k³ + k⁵ is prime
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- small‑primality helper for 128‑bit integers -------------------------------- */
static inline unsigned __int128 mulmod128(unsigned __int128 a,
                                          unsigned __int128 b,
                                          unsigned __int128 mod)
{
    unsigned __int128 result = 0;
    a %= mod;
    while (b)
    {
        if (b & 1)
        {
            result += a;
            if (result >= mod)
                result -= mod;
        }
        a <<= 1;
        if (a >= mod)
            a -= mod;
        b >>= 1;
    }
    return result;
}

static inline unsigned __int128 powmod128(unsigned __int128 a,
                                          unsigned __int128 e,
                                          unsigned __int128 mod)
{
    unsigned __int128 res = 1;
    a %= mod;
    while (e)
    {
        if (e & 1)
            res = mulmod128(res, a, mod);
        a = mulmod128(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* deterministic Miller–Rabin for 128‑bit numbers
   (bases 2,3,5,7,11,13,17,19,23,29,31,37 guarantee correctness in [2,2⁶⁴])
*/
static bool is_prime_128(unsigned __int128 n)
{
    if (n < 2)
        return false;
    if (n <= 3)
        return true;
    if ((n & 1) == 0)
        return n == 2;

    /* quick small‑prime test */
    static const uint64_t primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,
                                      23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(primes) / sizeof(primes[0]); ++i)
    {
        uint64_t p = primes[i];
        if (n == p)
            return true;
        if (n % p == 0)
            return false;
    }

    /* Miller–Rabin */
    unsigned __int128 d = n - 1;
    int s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    for (size_t i = 0; i < sizeof(primes) / sizeof(primes[0]); ++i)
    {
        uint64_t a0 = primes[i];
        if (a0 >= n) /* skip bases not needed */
            continue;

        unsigned __int128 x = powmod128((unsigned __int128)a0, d, n);
        if (x == 1 || x == n - 1)
            continue;

        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = mulmod128(x, x, n);
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

/* -------------------------------------------------------------------------- */
/* Main sequence function                                                    */
seq_t a(idx_t n)
{
    if (n == 0)  /* behaviour outside the OEIS offset is undefined */
        return 0;

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* next even integer to test */
    static idx_t next_candidate = 2;

    /* allocate cache on first call */
    if (!cache)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* maximum value representable in seq_t */
    seq_t seq_t_max = (seq_t)-1;

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n)
    {
        if (next_candidate & 1U)          /* skip odd numbers */
        {
            ++next_candidate;
            continue;
        }

        /* compute 1 + k + k³ + k⁵ using 128‑bit arithmetic */
        unsigned __int128 k = (unsigned __int128)next_candidate;
        unsigned __int128 k2 = k * k;                 /* k² */
        unsigned __int128 k3 = k2 * k;                /* k³ */
        unsigned __int128 k5 = k3 * k2;               /* k⁵ */
        unsigned __int128 sum = 1 + k + k3 + k5;

        if (is_prime_128(sum))
        {
            /* store only if it fits in seq_t */
            if ((unsigned __int128)next_candidate <= (unsigned __int128)seq_t_max)
            {
                if (cache_size >= cache_cap)
                {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp)
                    {
                        free(cache);
                        return 0;   /* allocation failure, unlikely in tests */
                    }
                    cache = tmp;
                }
                cache[cache_size++] = (seq_t)next_candidate;
            }
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
