/*
 * A153740  Smallest non-twin prime > n-th and (n+1)-th twin primes.
 * https://oeis.org/A153740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest non‑twin prime greater than the larger prime of
 *            each twin prime pair; duplicates are skipped. The sequence
 *            starts at a(1)=23. The definition follows the OEIS entry A153740
 * Range:     u8 n <= 10, u16 n <= 743, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- 64‑bit Miller–Rabin primality test ---------- */

static uint64_t _mult_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    /* multiply a and b modulo mod using 128‑bit arithmetic */
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t _pow_mod(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d)
    {
        if (d & 1) result = _mult_mod(result, a, mod);
        a = _mult_mod(a, a, mod);
        d >>= 1;
    }
    return result;
}

static bool _is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41,43,47
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    /* Decompose n-1 = d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = _pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = _mult_mod(x, x, n);
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static uint64_t _next_prime_gt(uint64_t n)
{
    if (n < 2) return 2;
    uint64_t p = n + 1;
    if ((p & 1) == 0 && p != 2) p++; /* ensure odd */
    while (!_is_prime_uint64(p))
    {
        p += 2;
        if (p < n) return 0; /* overflow guard */
    }
    return p;
}

/* ----------  Sequence a(n) implementation ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but safe */

    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* cache keeps all previously computed terms */

    /* persistent state for generating terms */
    static bool init = false;
    static uint64_t prev_prime = 0;   /* smaller prime of current pair */
    static uint64_t curr_prime = 0;   /* larger prime of current pair */
    static uint64_t prev_output = 0;  /* last value returned */

    if (!init)
    {
        /* initialise on first call */
        prev_prime = 3;
        curr_prime = 5;          /* first twin pair (3,5) */
        prev_output = 0;
        init = true;
    }

    /* grow cache as needed */
    if (cache_len < (size_t)n)
    {
        while (cache_len < (size_t)n)
        {
            /* search for next twin pair */
            while (true)
            {
                /* Check whether current pair is a twin */
                if (curr_prime - prev_prime == 2)
                {
                    /* Candidate must be > curr_prime (the larger prime) */
                    uint64_t cand = _next_prime_gt(curr_prime);
                    /* Skip any twin primes */
                    while (_is_prime_uint64(cand - 2) || _is_prime_uint64(cand + 2))
                        cand = _next_prime_gt(cand);

                    if (cand > prev_output)   /* new distinct value */
                    {
                        /* store it */
                        if (cache_len == cache_cap)
                        {
                            cache_cap = cache_cap ? cache_cap * 2 : 16;
                            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                        }
                        cache[cache_len++] = (seq_t)cand;
                        prev_output = cand;
                        /* advance to next prime after this pair */
                        prev_prime = curr_prime;
                        curr_prime = _next_prime_gt(curr_prime);
                        break;              /* term added, exit inner loop */
                    }
                    /* duplicate, skip adding but continue searching */
                }
                /* move to next prime (not part of a twin) */
                prev_prime = curr_prime;
                curr_prime = _next_prime_gt(curr_prime);
            }
        }
    }

    return cache[n - 1];
}
