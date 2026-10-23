/*
 * A031457  Numbers whose base-3 representation has one fewer 0 than 2's.
 * https://oeis.org/A031457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) counts numbers whose base‑3 representation has one fewer 0
 *            than 2. This implementation lazily generates terms, caching them
 *            so that calls in arbitrary order remain O(1) after the first
 *            fill
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   special_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/special_number.h"

/* Return the n‑th term of OEIS A031457.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Static cache: terms[0] holds a(1), terms[1] holds a(2), etc. */
    static seq_t *terms = NULL;
    static size_t   cache_size = 0;   /* number of terms stored */
    static size_t   cache_cap   = 0;   /* allocated capacity */
    static seq_t   next_value = 1;     /* integer to test next */

    /* Ensure we have at least n terms cached. */
    while (cache_size < (size_t)n) {
        /* Grow the cache if needed. */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) abort();        /* allocation failed */
            terms = tmp;
            cache_cap = new_cap;
        }

        /* Find the next qualifying number. */
        while (!special_number(next_value))
            ++next_value;

        /* Store it and advance the state. */
        terms[cache_size] = next_value;
        ++cache_size;
        ++next_value;
    }

    /* Return the desired term. */
    return terms[(size_t)n - 1];
}
