/*
 * A043877  Numbers k such that 0 and 1 occur juxtaposed in the base-3
 *          representation of k but not of k+1.
 * https://oeis.org/A043877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_adjacent_01or10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043877: numbers k such that the base‑3 representation of k
   contains the adjacent digits 01 or 10, but the representation of k+1
   does not.  The function searches sequentially and caches results
   to support calls in any order. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adjacent_01or10.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;      /* number of terms cached */
    static size_t cache_cap = 0;      /* capacity of cache array */
    static seq_t next_k = 0;          /* next integer to test */

    /* Expand cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Search for the next qualifying k */
        for ( ; ; ++next_k) {
            seq_t k_val = next_k;
            if (has_adjacent_01or10(k_val) && !has_adjacent_01or10(k_val + 1)) {
                /* Store the term */
                if (cache_len >= cache_cap) {
                    cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) /* allocation failed – abort */
                        abort();
                    cache = tmp;
                }
                cache[cache_len++] = k_val;
                ++next_k; /* move past the found value for the next search */
                break;    /* move to next required term */
            }
        }
    }
    return cache[(size_t)(n - 1)];
}
