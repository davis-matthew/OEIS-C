/*
 * A121068  Numbers k such that 8*k^2 + 7 is prime.
 * https://oeis.org/A121068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 11, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A121068: numbers k such that 8*k^2 + 7 is prime.
   The sequence a(1) = 0, a(2) = 3, a(3) = 27, …   (OEIS offset 1).
   The function finds the n‑th such k by brute‑force search, memoising
   all results found so far. */

/* The type seq_t is defined by the build (uint8_t … unsigned __int128
   or an arbitrary‑precision integer). idx_t is an unsigned 64‑bit index. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Use the generic big‑integer primality test supplied by the build. */
#include "../utils/is_prime_big.h"

/* Static cache of found k values. */
static seq_t *cache      = NULL;   /* array of k values */
static size_t cache_size = 0;      /* number of entries stored */
static size_t cache_cap  = 0;      /* allocated capacity of cache  */
static idx_t next_k      = 0;      /* next candidate k to test */

seq_t a(idx_t n)
{
    if (n == 0) {   /* undefined for n < 1 – return 0 as a safe value */
        return 0;
    }

    /* Generate terms until the cache contains at least n entries. */
    while (cache_size < n) {
        /* Compute 8*next_k^2 + 7 in seq_t to avoid overflow. */
        seq_t val = (seq_t)8 * (seq_t)next_k * (seq_t)next_k + (seq_t)7;

        if (is_prime_big(val)) {
            /* Expand the cache if necessary. */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 128;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            /* Store the found k (cast to seq_t); overflow is harmless
               for builds with small seq_t, and for larger builds it
               preserves the correct value. */
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;   /* move to the next candidate */
    }

    return cache[n - 1];
}
