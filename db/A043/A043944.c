/*
 * A043944  Numbers k such that 1 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent_1_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for numbers k whose octal representation contains
   adjacent digits 1 and 7 (either 17 or 71) but not k+1.
   The routine stores found terms in a cache so that calls in any order
   are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_1_7.h"

/* Cache of terms found so far. */
static seq_t *cache       = NULL;
static size_t cache_count = 0;
static size_t cache_cap   = 0;
static seq_t next_k       = 1;

/* Return the n-th element of A043944 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined, but harmless */

    /* Generate terms until we have at least n of them. */
    while (cache_count < (size_t)n) {
        seq_t k = next_k;
        if (has_adjacent_1_7(k) && !has_adjacent_1_7(k + 1)) {
            if (cache_count >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();            /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_count++] = k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
