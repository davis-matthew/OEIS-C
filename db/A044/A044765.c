/*
 * A044765  Numbers n such that string 5,2 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044765: numbers n that contain the decimal substring "52" but n+1 does not.
   The routine searches sequentially, caching the results for later calls. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Check whether decimal representation of x contains the substring "52". */
static bool contains_52(seq_t x)
{
    /* The substring "52" requires at least two digits. */
    if (x < 10) return false;

    int prev = -1;  /* previous (more significant) digit when scanning from LSB */
    while (x > 0) {
        unsigned long long d = (unsigned long long)(x % 10); /* current LSB digit */
        if (prev == 2 && d == 5) return true;                /* seen "5" after "2" in reverse -> original "52" */
        prev = (int)d;
        x /= 10;
    }
    return false;
}

/* Return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache        = NULL;
    static size_t cache_size   = 0;   /* allocated elements */
    static size_t cache_used   = 0;   /* number of terms stored */
    static seq_t candidate     = 52;  /* next number to check (initialized once) */

    /* Allocate the cache on the first call. */
    if (cache == NULL) {
        cache_size = 16;
        cache = mem_new(seq_t, cache_size);
        if (cache == NULL) abort();  /* unrecoverable */
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_used < n) {
        /* Find the next integer that satisfies the condition. */
        while (!(contains_52(candidate) && !contains_52(candidate + (seq_t)1))) {
            candidate++;
        }

        /* Store the found term. */
        if (cache_used == cache_size) {
            cache_size *= 2;
            seq_t *tmp = mem_resize(seq_t, cache, cache_size);
            if (tmp == NULL) abort();
            cache = tmp;
        }
        cache[cache_used++] = candidate++;
    }

    return cache[n - 1];
}
