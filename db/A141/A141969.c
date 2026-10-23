/*
 * A141969  Primes congruent to 11 mod 28.
 * https://oeis.org/A141969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 11 mod 28
 * Range:     u8 n <= 4, u16 n <= 544, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

static seq_t *cache = NULL;      /* cached primes of this sequence   */
static size_t cache_size = 0;    /* number of primes cached          */
static size_t cache_cap   = 0;    /* allocated capacity               */
static idx_t candidate;          /* next candidate to test           */
static bool initialized = false;

/* grow the cache to at least 'needed' capacity */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_mem)
        abort(); /* the build environment guarantees enough heap */

    if (cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_mem[i] = cache[i];
        free(cache);
    }

    cache = new_mem;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* OEIS offset is 1; return 0 for safety */
        return 0;

    if (!initialized) {
        candidate = 11;        /* first integer of the progression */
        initialized = true;
    }

    /* generate primes until we have at least n of them */
    while (cache_size < n) {
        if (is_prime(candidate)) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)candidate;
        }
        candidate += 28;
    }

    return cache[n - 1];
}
