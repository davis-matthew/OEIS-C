/*
 * A167777  Even single (or even isolated) numbers.
 * https://oeis.org/A167777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n≥2, a(n)=p+1 where p and p+2 are prime (the *
 *            average of the nth twin prime pair)
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Helper: dynamic array of primes (uint32_t)                          */
/* ------------------------------------------------------------------ */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

static void add_prime(uint32_t p)
{
    if (primes_cnt == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 32;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = p;
}

/* Integer floor sqrt for 32‑bit values */
static inline uint32_t isqrt_u32(uint32_t x)
{
    uint32_t r = 0;
    while ((uint64_t)(r + 1) * (r + 1) <= x) ++r;
    return r;
}

/* Simple primality test using the primes generated so far */
static bool is_prime(uint32_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if (x % 2 == 0) return false;
    uint32_t lim = isqrt_u32(x);
    for (size_t i = 0; i < primes_cnt && primes[i] <= lim; ++i) {
        if (x % primes[i] == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Sequence storage                                                */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* Next odd integer to test for primeness (starts at 3) */
static uint32_t next_candidate = 3;

/* Ensure that cache contains at least n terms.  Creates primes
 * incrementally and records averages of twin prime pairs. */
static void ensure_cache(idx_t n)
{
    if (!cache) {                 /* first call: initialise structures   */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)2;       /* a(1) = 2 */
        cache_len = 1;

        primes_cap = 32;
        primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) abort();
        primes_cnt = 1;
        primes[0] = 2;             /* prime 2 */
    }

    while (cache_len < n) {
        /* Test current odd candidate */
        if (is_prime(next_candidate)) {
            /* Found a prime: add to primes list */
            add_prime(next_candidate);

            uint32_t cand2 = next_candidate + 2;
            if (cand2 > 0 && is_prime(cand2)) {   /* avoid overflow         */
                /* Twin pair found: add average */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)(next_candidate + 1);
            }
        }
        next_candidate += 2;  /* skip even numbers */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for non‑positive indices */
    ensure_cache(n);
    return cache[n - 1];
}
