/*
 * A043168  Numbers k such that 2 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that base‑8 representation of k
 *            contains the adjacent pair 26 (or 62) while the representation
 *            of k−1 does not
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent_26_or_62
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_adjacent_26_or_62.h"

seq_t a(idx_t n)
{
    /* static cache of found terms, grows on demand */
    static seq_t *cache     = NULL;
    static size_t cache_size   = 0;
    static size_t cache_capacity = 0;

    /* static state for incremental search */
    static idx_t next_k = 1;   /* next candidate k to examine */
    static bool prev_has = false; /* whether k‑1 had the pattern */

    /* expand cache until it holds at least n entries */
    while (cache_size < (size_t)n) {
        seq_t k = (seq_t)next_k;
        bool curr_has = has_adjacent_26_or_62(k);

        if (curr_has && !prev_has) {
            if (cache_size == cache_capacity) {
                size_t new_cap = cache_capacity ? cache_capacity << 1 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)           /* allocation failure – return 0 */
                    return 0;
                cache = tmp;
                cache_capacity = new_cap;
            }
            cache[cache_size++] = k;
        }

        prev_has = curr_has;
        next_k++;              /* proceed to next integer */
    }

    return cache[n - 1];        /* a(1) is stored at index 0 */
}
