/*
 * A044628  Numbers n such that string 7,4 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_74_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044628: numbers n such that the base‑8 digits “7,4” appear in n
   but not in n+1.  The function computes the nth term using a simple
   incremental search and caches all previously found terms. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_74_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    static idx_t max_cached = 0;
    static size_t cache_cap = 0;
    static seq_t *cache = NULL;

    /* grow the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* return cached value if already known */
    if (n <= max_cached) return cache[n - 1];

    /* start searching from the smallest integer after the last found term */
    seq_t candidate = (max_cached == 0) ? 1 : cache[max_cached - 1] + 1;

    /* find terms until we reach the nth term */
    for (idx_t pos = max_cached + 1; pos <= n; ++pos) {
        while (true) {
            if (has_74_seq(candidate) && !has_74_seq(candidate + 1)) {
                cache[pos - 1] = candidate;
                ++candidate;   /* next search starts after this value */
                break;
            }
            ++candidate;
        }
    }

    max_cached = n;
    return cache[n - 1];
}
