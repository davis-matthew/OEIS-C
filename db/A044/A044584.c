/*
 * A044584  Numbers n such that string 2,0 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has20_octal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: term n is the nth integer m whose octal representation
   contains the substring “20” but the octal representation of m+1
   does not. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has20_octal.h"

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1)              /* index 0 is undefined */
        return 0;

    /* Static cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static seq_t last_checked = 0;   /* last integer examined */

    while (cache_size < (size_t)n) {
        seq_t candidate = last_checked + 1;
        last_checked = candidate;

        if (has20_octal(candidate) && !has20_octal(candidate + 1)) {
            /* Grow array if needed. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    exit(1);          /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
    }
    return cache[(size_t)n - 1];
}
