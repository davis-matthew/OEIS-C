/*
 * A129492  Composite numbers k such that 2^k mod k is a power of 2.
 * https://oeis.org/A129492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: composite k such that 2^k mod k is a power of 2 (OEIS A129492) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t) */
#include "../utils/is_power_of_two.h"   /* bool is_power_of_two(seq_t) */
#include "../utils/mod_pow.h"           /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */

/* ---------- Static cache to memoise terms ---------- */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_size = 0;      /* number of valid terms stored */
static size_t cache_cap  = 0;      /* allocated capacity of cache */
static idx_t  last_checked = 5;    /* last integer examined (start before first candidate 6) */

/* Grow the cache array if a capacity of at least 'min_cap' is required. */
static void ensure_cache_cap(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(EXIT_FAILURE);   /* out of memory */
    cache = new_arr;
    cache_cap = new_cap;
}

/* ---------- Main function required by the build ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* behaviour below the offset does not matter */

    /* Ensure the cache has space for at least 'n' terms. */
    size_t needed = (size_t)n;
    ensure_cache_cap(needed);

    /* Generate further terms until we have 'n' of them. */
    while (cache_size < needed) {
        last_checked++;                     /* move to next integer candidate */

        /* Skip primes – we only want composites. */
        if (is_prime(last_checked))
            continue;

        /* Compute 2^k mod k. */
        seq_t mod_res = mod_pow((seq_t)2, (uint64_t)last_checked, (seq_t)last_checked);

        /* Keep the term if the result is a power of two. */
        if (is_power_of_two(mod_res)) {
            cache[cache_size++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];   /* a(1) is the first stored term, etc. */
}
