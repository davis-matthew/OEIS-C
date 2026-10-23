/*
 * A179082  Even numbers having an even sum of digits in their decimal
 *          representation.
 * https://oeis.org/A179082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits_sum_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Even numbers having an even sum of decimal digits (OEIS A179082) */
/* This implementation generates the sequence on demand, caching results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_sum_even.h"

seq_t a(idx_t n)
{
    /* Cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored in cache */
    static size_t cache_cap = 0;   /* current allocation capacity */
    /* Next even candidate to test */
    static seq_t candidate = 0u;

    /* Ensure cache is large enough to hold the requested index */
    if (n > cache_len) {
        /* Grow cache if necessary */
        if (cache_cap < n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 128;
            if (new_cap < (size_t)n) new_cap = (size_t)n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE); /* Allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Generate terms until we reach the desired index */
        while (cache_len < (size_t)n) {
            if (digits_sum_even(candidate)) {
                cache[cache_len++] = candidate;
            }
            candidate += 2u; /* next even number */
        }
    }
    /* Return the requested term (1‑based index) */
    return cache[n - 1u];
}
