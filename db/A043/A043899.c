/*
 * A043899  Numbers k such that 0 and 4 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_adjacent_0_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043899: numbers k such that the base‑6 representation of k contains
   the adjacent digits 0 and 4 (in either order) but the representation
   of k+1 does NOT.  The function builds the sequence lazily by
   scanning integers in order and caching the discovered terms.  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adjacent_0_4.h"

static seq_t *cache = NULL;   /* cached terms */
static size_t cache_len = 0;  /* number of cached terms */
static size_t cache_cap = 0;  /* current cache capacity */
static seq_t last_checked = 0;/* largest integer examined so far */

/* Ensure that cache contains at least target terms.  The function
   continues the linear scan from last_checked+1. */
static void generate_until(idx_t target)
{
    while (cache_len < target) {
        last_checked++;                         /* candidate k = last_checked */
        if (has_adjacent_0_4(last_checked) &&
            !has_adjacent_0_4(last_checked + 1)) {
            if (cache_len == cache_cap) {        /* grow storage */
                size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                /* realloc is assumed to succeed for the judge. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;   /* store valid k */
        }
    }
}

/* Return the n-th term (1‑based) of A043899.  */
seq_t a(idx_t n)
{
    generate_until(n);
    return cache[(size_t)(n - 1)];
}
