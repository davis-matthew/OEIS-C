/*
 * A084050  Numbers n such that at least one permutation of the digits of n
 *          yields a palindrome.
 * https://oeis.org/A084050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_permutable_to_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate numbers in increasing order and return the
 * nth element of sequence A084050.  A number n is accepted if
 * some permutation of its decimal digits (zeros allowed at the front
 * and then dropped) yields a palindrome.  Equivalently, after
 * removing an arbitrary number of leading zeros, the remaining
 * digits can be rearranged to form a palindrome, i.e. at most
 * one digit has an odd occurrence. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_permutable_to_palindrome.h"

/* a(n): nth term of A084050 (1‑based).  The function lazily
 * computes terms and caches them to avoid recomputation on
 * subsequent calls. */
seq_t a(idx_t n) {
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;    /* number of terms stored */
    static idx_t cache_cap  = 0;    /* allocated capacity */
    static seq_t last_checked = (seq_t)0;  /* last integer examined */

    /* Ensure cache has capacity for at least n terms. */
    while (cache_size < n) {
        /* Advance to next integer. */
        ++last_checked;

        if (is_permutable_to_palindrome(last_checked)) {
            /* Store the term. */
            if (cache_size == cache_cap) {
                /* Grow cache (double). */
                size_t new_cap = (cache_cap == 0) ? 128 : (size_t)cache_cap * 2;
                seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_buf) {
                    /* Allocation failure – abort with zero. */
                    return (seq_t)0;
                }
                cache = new_buf;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
    }

    /* Return the requested term. */
    return cache[n - 1];
}
