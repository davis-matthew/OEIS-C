/*
 * A067635  a(1) = 1; set of digits of a(n)^2 is a proper subset of the set of
 *          digits of a(n+1)^2.
 * https://oeis.org/A067635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   digit_present, proper_subset
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067635: a(1) = 1; for n > 1,
   a(n) is the smallest integer > a(n-1) such that the set of decimal
   digits of a(n-1)^2 is a proper subset of the set of decimal digits
   of a(n)^2. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_present.h"
#include "../utils/proper_subset.h"

/* Compute a(n). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n < 1, but safe */

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t size = 0;

    /* Initialise cache with a(1) = 1 if it does not exist yet */
    if (capacity == 0) {
        capacity = 4;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(1) */
        size = 1;
    }

    /* Generate terms until we have enough */
    while (size < n) {
        seq_t prev = cache[size - 1];

        bool prevPresent[10];
        digit_present(prev * prev, prevPresent);

        seq_t candidate = prev + (seq_t)1;
        while (true) {
            seq_t sqCand = candidate * candidate;
            bool candPresent[10];
            digit_present(sqCand, candPresent);
            if (proper_subset(prevPresent, candPresent)) {
                /* Append candidate */
                if (size >= capacity) {
                    capacity *= 2;
                    cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
                }
                cache[size] = candidate;
                ++size;
                break;
            }
            ++candidate;
        }
    }

    return cache[n - 1];
}
