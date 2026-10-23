/*
 * A191085  Primes p that have Kronecker symbol (p|91) = -1.
 * https://oeis.org/A191085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191085:
   primes p such that the Kronecker (Jacobi) symbol (p|91) = -1. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/jacobi_symbol.h"
#include "../utils/isPrimeGeneric.h"

/* Cache of primes satisfying the condition */
static seq_t *cache    = NULL;   /* array of primes found */
static size_t cache_len = 0;     /* current number of cached primes */
static size_t cache_cap = 0;     /* capacity of the cache array */
static uint64_t next_candidate = 2; /* next integer to test for primality */
static bool initialized = false;

/* Helper to grow the cache when needed */
static void grow_cache(size_t min_cap) {
    if (cache_cap < min_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Ensure cache is initialized */
    if (!initialized) {
        initialized = true;
        cache_cap   = 128;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Generate primes with the desired property until we have n of them */
    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;

        /* Handle the first prime 2 specially */
        if (p > 2 && (p & 1) == 0) {
            p++; /* make odd */
        }

        /* Find next prime */
        while (!isPrimeGeneric((seq_t)p)) {
            p += 2; /* skip even numbers */
        }

        /* Check the Jacobi symbol condition */
        if (jacobi_symbol(p, 91) == -1) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)p;
        }

        /* Set up next candidate */
        next_candidate = (p == 2) ? 3 : p + 2;
    }

    return cache[(size_t)n - 1];
}
