/*
 * A044329  Numbers n such that string 8,6 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_substring_86
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search-based implementation of OEIS A044329:
   Numbers n such that the base‑9 representation of n contains
   the substring "86" but the representation of n‑1 does not.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_substring_86.h"

/* Static cache state */
static seq_t *cache      = NULL;   /* array of found terms */
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* current capacity of cache */
static seq_t last_checked = 0;     /* last integer examined */

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)         /* undefined for index 0 – return 0 */
        return (seq_t)0;

    /* Expand cache capacity if necessary */
    if (cache_size < (size_t)n) {
        if (cache_cap == 0) {
            cache_cap = 64;                        /* initial capacity */
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else {
            while (cache_cap < (size_t)n)
                cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Search for new terms until we have n of them */
        while (cache_size < (size_t)n) {
            seq_t m = last_checked + 1;

            if (has_substring_86(m) &&
                !has_substring_86(m - 1)) {
                cache[cache_size++] = m;
            }

            last_checked = m;
        }
    }

    return cache[(size_t)n - 1];
}
