/*
 * A097102  Numbers m that are the hypotenuse of exactly 13 distinct
 *          integer-sided right triangles, i.e., m^2 can be written as a sum
 *          of two squares in 13 ways.
 * https://oeis.org/A097102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 924, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A097102: hypotenuse of exactly 13 distinct integer‑sided right triangles.
    For a number m, let m = ∏ pᵉ with p prime.  
    The number of representations of m² as a sum of two squares is 
    R = ∏_{p ≡ 1 (4)} (2e + 1).  
    A hypotenuse yields 13 distinct triangles iff R = 27. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------  Prime generation  ---------- */
static uint32_t      *primes      = NULL;   /* list of primes       */
static size_t        primes_size = 0;       /* number of primes     */
static size_t        primes_cap  = 0;       /* array capacity       */
static uint32_t       next_candidate = 2;    /* next number to test */

/* Ensure that primes contains all primes whose square is ≤ limit. */
static void ensure_primes_upto(uint64_t limit)
{
    if (primes_size == 0) {
        /* allocate storage for initial primes */
        primes_cap = 8;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
        primes[primes_size++] = 2;
    }
    /* Grow primes until the square of the last prime exceeds limit. */
    while ((uint64_t)next_candidate * next_candidate <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_size; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > next_candidate) break;
            if (next_candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_size == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_size++] = next_candidate;
        }
        if (next_candidate == 2) next_candidate = 3;
        else next_candidate += 2;
    }
}

/* ----------  Count representations  ---------- */
static int count_triples(uint64_t m)
{
    uint64_t temp = m;
    int prod = 1;

    /* Ensure primes up to sqrt(temp) are available. */
    ensure_primes_upto(temp);

    for (size_t i = 0; i < primes_size && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        int exp = 0;
        while (temp % p == 0) { temp /= p; ++exp; }
        if (exp > 0 && (p & 3) == 1) {          /* p ≡ 1 (mod 4) */
            prod *= (2 * exp + 1);
            if (prod > 27) return 0;            /* early exit */
        }
    }
    if (temp > 1) {                              /* remaining prime factor */
        if ((temp & 3) == 1) {
            prod *= 3;                          /* (2·1+1) = 3 */
            if (prod > 27) return 0;
        }
    }
    return (prod == 27) ? 13 : 0;               /* exactly 13 triangles? */
}

/* ----------  Cache for sequence terms  ---------- */
static seq_t *cache = NULL;     /* stored terms */
static size_t cache_size = 0;   /* number of cached terms */
static size_t cache_cap  = 0;   /* array capacity              */

/* Append one term to the cache. */
static void add_term(seq_t val)
{
    if (cache_size == cache_cap) {
        cache_cap = (cache_cap == 0) ? 8 : (cache_cap << 1);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_size++] = val;
}

/* ----------  Main sequence function  ---------- */
static uint64_t current_m = 1;   /* next candidate m */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    while (cache_size < (size_t)n) {
        if (count_triples(current_m) == 13) {
            add_term((seq_t)current_m);
        }
        ++current_m;
    }
    return cache[n - 1];
}
