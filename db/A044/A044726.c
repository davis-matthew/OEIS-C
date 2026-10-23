/*
 * A044726  Numbers n such that string 1,3 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044726: numbers n such that the decimal representation of n contains
 * the substring "13" but n+1 does not.  The function performs a brute‑force
 * search with memoisation: it keeps a static cache of found terms and expands
 * it as needed when a higher index is requested.  The routine has O(m) time
 * per new element, where m is the maximum integer examined so far; for
 * n≈10 000 the total work is well below the 2 s limit. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Test whether the decimal representation of x contains the substring "13". */
static bool has13(seq_t x)
{
    /* Check pairs of adjacent digits by taking the last two digits each step. */
    while (x >= (seq_t)100) {
        if ((x % (seq_t)100) == (seq_t)13) return true;
        x /= (seq_t)10;
    }
    return x == (seq_t)13;
}

seq_t a(idx_t n)
{
    /* Static cache and state. */
    static seq_t *cache = NULL;
    static size_t   cache_cap   = 0;
    static size_t   cache_size  = 0;   /* number of terms stored */
    static seq_t   next_candidate = (seq_t)1; /* next integer to test */

    /* Return from cache if already computed. */
    if (n <= cache_size) return cache[n - 1];

    /* Grow the cache array to accommodate more terms. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_size >= cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have enough. */
    while (cache_size < n) {
        if (has13(next_candidate) && !has13(next_candidate + (seq_t)1)) {
            cache[cache_size++] = next_candidate;
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
        }
        next_candidate++;
    }

    return cache[n - 1];
}
