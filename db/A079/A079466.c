/*
 * A079466  Numbers k such that the "inventory" A063850 of k is a palindrome.
 * https://oeis.org/A079466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   inventory_is_pal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079466: numbers k whose inventory A063850(k) is a palindrome.
   Computation: scan integers sequentially, testing each one.  The routine
   keeps a cache of found terms so that arbitrary order of calls is
   supported.  All computations use only integer arithmetic on seq_t. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/inventory_is_pal.h"

/* Sequence entry function.  Uses a cache of previously found terms. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* dynamic array of terms */
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t next = (seq_t)1;      /* next integer to test */

    if (n == 0) return (seq_t)0;       /* safety, though sequence starts at 1 */

    /* Grow cache if necessary. */
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – return 0 as error sentinel */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Find terms up to index n. */
    while (cache_size < (size_t)n) {
        if (inventory_is_pal(next)) {
            cache[cache_size++] = next;
        }
        /* Increment next using addition to stay within allowed operators. */
        next = next + (seq_t)1;
    }
    return cache[n - 1];
}
