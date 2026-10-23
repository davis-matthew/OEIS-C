/*
 * A044609  Numbers n such that string 5,1 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_51_octal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044609
 *
 * Definition: a(n) is the nth positive integer such that the octal
 * representation of n contains the substring "51" but the octal
 * representation of n+1 does NOT contain "51".
 *
 * Implementation: we simply generate the sequence by scanning
 * integers in increasing order, testing the required property.
 * The function keeps a static cache so that calls in arbitrary
 * order are answered efficiently.  The cache is grown as needed.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_51_octal.h"

/* Main function required by the platform */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 1;   /* next integer to test */

    /* grow cache at start */
    if (cache == NULL) {
        cache_cap = 32;                /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */
    }

    /* generate required terms until we have at least n of them */
    while (cache_len < n) {
        seq_t cur = (seq_t)next_candidate;
        if (has_51_octal(cur) && !has_51_octal(cur + 1)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) return 0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
