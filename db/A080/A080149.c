/*
 * A080149  Numbers k such that k^2 + 1 and k^2 + 3 are both prime.
 * https://oeis.org/A080149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 497, u32 n <= 9952, u64 n <= 9952,
 *            u128 n <= 9952, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate the sequence by testing successive integers k for
   the condition that k^2+1 and k^2+3 are both prime.  The sequence is
   1‑based: a(1)=2.  The algorithm stores found terms in a static cache
   so that subsequent calls are fast. */

/* Standard headers – available in both C11 and C++17 */
#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t, size_t */
#include <stdbool.h>  /* bool */

/*======================================================================
 * 32‑bit and 64‑bit helper functions
 *====================================================================*/
#if defined(__SIZEOF_INT128__) || defined(_MSC_VER) && defined(_M_X64)
/* use 128‑bit arithmetic for modular multiplication */
static inline uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}
#else
/* fallback for platforms without uint128_t – not expected in test harness */
static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    uint64_t res = 0;
    while (b)
    {
        if (b & 1) res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
}
#endif

/* Miller–Rabin primality test for 64‑bit integers – deterministic */
static bool isPrime64(uint64_t n)
{
    if (n < 2)                   return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes/sizeof small_primes[0]; ++i)
    {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases/sizeof bases[0]; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = 1;
        uint64_t pow = d;
        uint64_t base = a;
        /* fast modular exponentiation */
        while (pow)
        {
            if (pow & 1) x = mulmod(x, base, n);
            base = mulmod(base, base, n);
            pow >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = mulmod(x, x, n);
            if (x == n-1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/*======================================================================
 *  Cache for found terms
 *====================================================================*/
static seq_t *terms = NULL;         /* array of terms */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* capacity of the array */
static idx_t tested_k = 0;           /* largest k that has been tested */

/* Ensure the cache has at least 'needed' capacity.  Uses a doubling strategy. */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/*======================================================================
 *  Main sequence function
 *====================================================================*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, but harmless */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n)
    {
        ++tested_k;
        uint64_t k = (uint64_t)tested_k;
        uint64_t k2 = k * k;           /* k^2 fits into uint64_t for all k needed */
        if (isPrime64(k2 + 1ULL) && isPrime64(k2 + 3ULL))
        {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = (seq_t)k;
        }
    }
    return terms[n - 1];                /* 1‑based indexing */
}
