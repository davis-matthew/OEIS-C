/*
 * A084720  Primes p such that p+1 is a product of two or more consecutive
 *          integers.
 * https://oeis.org/A084720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that p+1 can be written as a
 *            product * of two or more consecutive integers. This is computed
 *            by testing each * successive prime for the property
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_consecutive_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_consecutive_product.h"

/* --- Small helpers ----------------------------------------------------- */

static inline seq_t mod_mul(seq_t a, seq_t b, seq_t mod)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    /* When seq_t is a builtin integer this avoids overflow. */
    return (seq_t)((__int128)a * b % mod);
#else
    return (a * b) % mod;
#endif
}

static inline seq_t mod_pow(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    while (exp > 0)
    {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin for 64‑bit integers and reasonably small
 * arbitrary‑precision values.  The base set 2,3,5,7,11,13,17,19,23,29,31,37
 * is sufficient for all numbers < 2^128. */
static bool is_prime_generic(seq_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* Write n-1 as d*2^s with d odd. */
    seq_t d = n - 1;
    seq_t s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    const unsigned long long bases[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,
        17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        seq_t a = (seq_t)bases[i];
        if (a % n == 0) continue;          /* a ≡ 0 mod n */

        seq_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool continue_outer = false;
        for (seq_t r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1)
            {
                continue_outer = true;
                break;
            }
        }
        if (continue_outer) continue;

        return false;                      /* composite */
    }
    return true;                           /* probably prime */
}

/* --- Caching of discovered terms --------------------------------------- */

static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
/* The first prime candidate to test. */
static seq_t next_candidate = 2;

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)                      /* safety: undefined behaviour */
        return (seq_t)0;

    /* Build cache until it contains at least n entries. */
    while (cache_len < n)
    {
        /* Skip non‑primes. */
        if (!is_prime_generic(next_candidate))
        {
            ++next_candidate;
            continue;
        }

        /* Check the consecutive‑product property on p+1. */
        if (is_consecutive_product(next_candidate + (seq_t)1))
        {
            if (cache_len >= cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
