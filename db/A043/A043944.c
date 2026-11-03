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
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for numbers k whose octal representation contains
   adjacent digits 1 and 7 (either 17 or 71) but not k+1.
   The routine stores found terms in a cache so that calls in any order
   are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the octal representation of x contains a pair of
   adjacent 1 and 7 in any order. */
static bool has_adjacent_1_7(seq_t x)
{
    if (x == 0)
        return false;
    unsigned int prev = 8;               /* sentinel – not an octal digit */
    while (x > 0) {
        unsigned int cur = (unsigned)(x % 8);
        if ((prev == 1 && cur == 7) || (prev == 7 && cur == 1))
            return true;
        prev = cur;
        x /= 8;
    }
    return false;
}

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
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
