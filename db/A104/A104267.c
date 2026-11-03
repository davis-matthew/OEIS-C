/*
 * A104267  Primes which are the concatenation of five consecutive square
 *          numbers.
 * https://oeis.org/A104267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes which are the concatenation of five consecutive square numbers.
   This implementation iterates over consecutive integers m, concatenates the
   squares m²,(m+1)²,…,(m+4)², tests whether the concatenated number is prime,
   and stores the prime.  The function a(n) returns the n-th such prime. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return the number of decimal digits of x. */
static idx_t digit_len(seq_t x)
{
    idx_t len = 0;
    seq_t t = x;
    while (t > 0) {
        t /= 10;
        len++;
    }
    return len;
}

static seq_t pow10_idx(idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= 10;
    return res;
}

/* Miller–Rabin primality test for arbitrary-precision values. */
static bool isprime_big(seq_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    seq_t d = n - 1;
    idx_t r = 0;
    while ((d & 1) == 0) {
        d = d / 2;
        r = r + 1;
    }

    /* Bases sufficient for a probabilistic test. */
    const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29};
    for (idx_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        seq_t a = (seq_t)bases[i];
        if (a >= n) break;

        /* modular exponentiation x = a^d mod n */
        seq_t x = 1;
        seq_t base = a % n;
        seq_t exp  = d;
        while (exp > 0) {
            if ((exp & 1) != 0) x = (x * base) % n;
            base = (base * base) % n;
            exp  = exp >> 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool cont = false;
        for (idx_t j = 1; j < r; ++j) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

/* Cached primes and state for memoization */
static seq_t *prime_cache = NULL;   /* dynamic array of primes seen so far */
static idx_t cache_size = 0;        /* number of primes stored */
static idx_t cache_cap   = 0;        /* allocated capacity of prime_cache */
static idx_t m_current   = 0;        /* last m processed */

/* Return the n-th term (prime) of the sequence, n ≥ 1. */
seq_t a(idx_t n)
{
    /* Ensure cache array is allocated */
    if (prime_cache == NULL) {
        cache_cap   = 16;
        prime_cache = seq_malloc(cache_cap);
        cache_size  = 0;
    }

    /* Grow the cache array if needed */
    if (cache_size + 1 > cache_cap) {
        cache_cap <<= 1;
        prime_cache = seq_realloc(prime_cache, cache_cap);
    }

    /* Generate primes until we have at least n of them */
    while (cache_size < n) {
        m_current++;    /* next starting square index */

        seq_t concat = 0;
        for (idx_t k = 0; k < 5; ++k) {
            seq_t sq = (seq_t)(m_current + k) * (seq_t)(m_current + k);
            idx_t  d  = digit_len(sq);
            seq_t  mult = pow10_idx(d);
            concat = concat * mult + sq;
        }

        if (isprime_big(concat)) {
            /* store the prime */
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                prime_cache = seq_realloc(prime_cache, cache_cap);
            }
            prime_cache[cache_size++] = concat;
        }
    }

    return prime_cache[n - 1];
}
