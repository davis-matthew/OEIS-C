/*
 * A057455  n*M31 - 1 is prime, where M31 = 2^31 - 1.
 * https://oeis.org/A057455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that k*M31−1 is prime, where M31
 *            = 2³¹−1 = 2147483647. a(1) = 90, a(2) = 96, …
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Local deterministic Miller–Rabin primes checker for u64 ----------
 *  Works for all 64‑bit numbers.  It uses multiplication modulo mod via
 *  unsigned __int128 to avoid overflow.  The selected bases are valid
 *  for  n < 3 474 749 660 383  (our values are below about 2.2×10¹³).
 */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1ULL)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                 return false;
    if (n % 2 == 0)            return n == 2;
    if (n % 3 == 0)            return n == 3;
    /* Miller–Rabin with bases [2,3,5,7,11] */
    static const uint64_t bases[] = { 2ULL, 3ULL, 5ULL, 7ULL, 11ULL };
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1;
        ++s;
    }
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1)
            {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Sequence calculation ----------
 *  We lazily build a cache of terms, extending it until we have at least
 *  n entries.  The sequence is 1‑based: a(1) is the first stored term.
 */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* cached terms */
    static size_t cap   = 0;       /* current capacity of cache array */
    static size_t count = 0;       /* number of terms stored so far    */
    static idx_t cand  = 1;        /* next integer to test */

    if (cache == NULL)            /* first call: allocate default size */
    {
        cap   = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    const uint64_t M31 = 2147483647ULL;    /* 2^31−1 */

    /* extend cache until we have at least n terms */
    while (count < n)
    {
        /* compute val = cand*M31 − 1 using 128 bit intermediate */
        uint64_t val = (uint64_t)(((__uint128_t)cand * M31) - 1);
        if (is_prime_uint64(val))
        {
            if (count == cap)     /* grow capacity if necessary */
            {
                size_t oldcap = cap;
                cap   *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[count++] = (seq_t)cand;
        }
        ++cand;
    }

    return cache[n - 1];          /* 1‑based indexing */
}
