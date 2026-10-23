/*
 * A023724  Numbers with exactly 3 0's in their base 5 expansion.
 * https://oeis.org/A023724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5185, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_three_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers with exactly 3 zero digits in their base‑5 expansion.
   Computation: brute‑force enumeration of integers and selection of those
   whose base‑5 representation contains exactly three zeros. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_three_zeros.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache for generated terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t next_num = 0;  /* last integer examined */

    /* Ensure cache has capacity for at least n terms */
    while (len < n) {
        /* Grow cache when needed */
        if (len == cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* allocation failed – abort gracefully */
                abort();
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* Find next qualifying number */
        ++next_num;
        if (has_three_zeros(next_num)) {
            cache[len++] = (seq_t)next_num;
        }
    }

    return cache[n - 1];   /* a(1) is first element */
}
