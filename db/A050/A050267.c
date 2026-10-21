/*
 * A050267  Primes or negative values of primes in the sequence b(n) = 47*n^2
 *          - 1701*n + 10181, n >= 0.
 * https://oeis.org/A050267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, less
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 44, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A050267: a(n) = b(m) where b(m)=47*m²-1701*m+10181 and |b(m)| is prime; 
   offset 1 (a(1) is the first such term). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrime.h"

/* Dynamic array to cache the prime‐valued terms */
static seq_t *cache      = NULL; /* stored terms b(m) that are +/- prime */
static size_t cache_sz = 0;     /* number of terms stored */
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t  next_m   = 0;     /* next m to test in b(m) */

/* Ensure the cache can hold at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failed – terminate (no I/O) */
    cache = tmp;
    cache_cap = new_cap;
}

/* Test b(m) for primality and, if positive, append it to cache. */
static void try_cache(idx_t m)
{
    /* Compute b(m) = 47*m² - 1701*m + 10181 */
    seq_t val = (seq_t)47 * (seq_t)m * (seq_t)m
              - (seq_t)1701 * (seq_t)m
              + (seq_t)10181;

    /* Absolute value of b(m) */
    seq_t abs_val = (val >= 0) ? val : -val;

    /* Convert to idx_t for isPrime.  For the range of interest
       the absolute value fits into the 64-bit unsigned type. */
    idx_t candidate = (idx_t)abs_val;
    if (isPrime(candidate)) {
        if (cache_sz == cache_cap) {
            grow_cache(cache_cap == 0 ? 8 : cache_cap * 2);
        }
        cache[cache_sz++] = val; /* store the signed value */
    }
}

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* defined only for n ≥ 1 */

    /* Generate terms until we have at least n values cached. */
    while (cache_sz < n) {
        try_cache(next_m);
        ++next_m;
    }
    return cache[n - 1];
}
