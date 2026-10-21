/*
 * A051804  Numbers with nonzero multiplicative digital root 2.
 * https://oeis.org/A051804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   has_mdr2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A051804 – Numbers with non‑zero multiplicative digital root 2.
 *
 * To determine if a number x has MDR 2 we repeatedly multiply its
 * non‑zero decimal digits until a single digit is obtained.
 * This function is used to test candidates while filling a cache.
 * The sequence term a(n) (1‑based) is returned by the function below.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_mdr2.h"

/* Return the nth term of A051804 (1‑based indexing).  Calls may arrive
 * in any order; the results are cached for efficiency. */
seq_t a(idx_t n)
{
    /* Cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t last_checked = 0;     /* largest integer that has been
                                         * inspected so far */

    /* Initialise cache lazily. */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        ++last_checked;
        if (has_mdr2((seq_t)last_checked)) {
            if (cache_size == cache_cap) {   /* grow cache when full */
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
