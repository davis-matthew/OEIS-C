/*
 * A212166  Numbers k such that the maximum exponent in its prime
 *          factorization equals the number of positive exponents (A051903(k)
 *          = A001221(k)).
 * https://oeis.org/A212166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A212166: numbers k such that the maximum exponent in its
   prime factorization equals the number of distinct prime factors.
   The function generates terms lazily, testing each integer by
   trial‑division with a growing list of primes. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root for 64‑bit values (floor). */
static uint32_t isqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = 4294967295ULL;          /* 2^32‑1 */
    while (lo < hi)
    {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (uint32_t)lo;
}

/* Dynamically growing array of primes (uint32_t). */
static uint32_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;
static uint32_t prime_next_candidate = 2;

/* Generate all primes up to 'limit' (inclusive). */
static void generate_primes_up_to(uint32_t limit)
{
    if (limit < prime_next_candidate)
        return;
    /* estimate needed capacity */
    size_t need = primes_len + (size_t)(limit - prime_next_candidate) / 2 + 10;
    if (need > primes_cap)
    {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        while (new_cap < need)
            new_cap *= 2;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp && new_cap > 0) exit(EXIT_FAILURE);
        primes = tmp;
        primes_cap = new_cap;
    }
    for (uint32_t x = prime_next_candidate; x <= limit; ++x)
    {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i)
        {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > x)
                break;
            if (x % p == 0)
            {
                prime = false;
                break;
            }
        }
        if (prime)
            primes[primes_len++] = x;
    }
    prime_next_candidate = limit + 1;
}

/* Return true if 'x' satisfies the defining property. */
static bool is_special(uint64_t x)
{
    if (x == 1)
        return true;
    uint64_t n = x;
    size_t distinct = 0;
    uint64_t max_exp = 0;
    uint32_t limit = isqrt_u64(n);
    generate_primes_up_to(limit);
    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * primes[i] <= n; ++i)
    {
        uint32_t p = primes[i];
        if (n % p == 0)
        {
            ++distinct;
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            if (e > max_exp)
                max_exp = e;
        }
    }
    if (n > 1) /* remaining prime factor */
    {
        ++distinct;
        if (1 > max_exp)
            max_exp = 1;
    }
    return max_exp == distinct;
}

/* Lazy storage of computed terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static uint64_t checked_up_to = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but harmless */

    while (terms_len < n)
    {
        ++checked_up_to;
        uint64_t cand = checked_up_to;
        if (is_special(cand))
        {
            if (terms_len == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp && new_cap > 0) exit(EXIT_FAILURE);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)cand;
        }
    }
    return terms[n - 1];
}
