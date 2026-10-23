/*
 * A176258  Numbers of the form 4k+1 with greatest prime divisor of the form
 *          4m-1.
 * https://oeis.org/A176258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 7124, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th integer ≡ 1 (mod 4) whose greatest prime divisor ≡ 3 (mod 4) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gpf.h"   /* declares uint64_t gpf(uint64_t) */

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cache_size = 0;      /* number of terms stored */
    static size_t cache_cap  = 0;      /* allocated capacity of cache */
    static uint64_t cur       = 5;      /* next candidate ≡ 1 (mod 4) */

    /* The sequence is defined for n ≥ 1.  For safety return 0 for n == 0. */
    if (n == 0) return (seq_t)0;

    /* Initialise the cache the first time the function is called */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Grow the cache if necessary */
    if (cache_size >= cache_cap) {
        size_t new_cap = cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        /* cur is always ≡ 1 (mod 4) as we step by 4 */
        uint64_t p = gpf(cur);          /* greatest prime factor of cur */
        if ((p & 3ULL) == 3ULL)         /* p ≡ 3 (mod 4)? */
            cache[cache_size++] = (seq_t)cur;

        cur += 4ULL;                    /* next candidate */
        /* If the cache becomes full while generating, grow it again */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    return cache[n - 1];
}
