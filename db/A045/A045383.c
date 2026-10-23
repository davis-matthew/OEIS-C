/*
 * A045383  Primes congruent to {0, 2} mod 7.
 * https://oeis.org/A045383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th prime that is congruent to 0 or 2 modulo 7
 * (primes ≡ 2 (mod 7) plus the special case 7 itself).
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- small modular arithmetic helpers ---------- */
static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    /* use 128-bit multiplication to avoid overflow */
    return (unsigned __int128)a * b % mod;
}

static uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1u)
            result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1u;
    }
    return result;
}

/* ---------- deterministic Miller–Rabin for 64‑bit integers ---------- */
static int isPrime64(uint64_t n)
{
    if (n < 2)
        return 0;

    /* small primes first */
    static const uint64_t small_primes[] =
        {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i)
    {
        if (n == small_primes[i])
            return 1;
        if (n % small_primes[i] == 0)
            return 0;
    }

    /* write n-1 as d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1u) == 0)
    {
        d >>= 1u;
        s++;
    }

    /* bases sufficient for determinism on 64‑bit integers */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0)
            continue;
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        int composite = 1;
        for (unsigned r = 1; r < s; ++r)
        {
            x = mulmod(x, x, n);
            if (x == n - 1)
            {
                composite = 0;
                break;
            }
        }
        if (composite)
            return 0;
    }
    return 1;
}

/* ---------- cached sequence storage ---------- */
static seq_t *cache = NULL;     /* the generated sequence values */
static size_t cache_len = 0;    /* number of values stored so far */
static size_t cache_cap = 0;    /* capacity of the cache array */

static uint64_t next_candidate = 2;  /* next integer to test for primality */

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* generate values until we have at least n elements */
    while (cache_len < (size_t)n)
    {
        uint64_t p = next_candidate;

        /* primality test */
        if (isPrime64(p))
        {
            /* include primes ≡ 2 (mod 7) and the prime 7 itself */
            if (p == 2 || p == 7 || (p % 7u == 2u))
            {
                if (cache_len >= cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)p;
            }
        }

        /* advance to the next candidate:
           after 2, start with 3 and then skip by 2 to stay odd */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return cache[n - 1];
}
