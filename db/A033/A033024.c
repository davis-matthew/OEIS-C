/*
 * A033024  Numbers whose base-11 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 214, u32 n <= 1300, u64 n <= 1300,
 *            u128 n <= 1300, big any n
 * Helpers:   valid_base11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth integer whose base‑11 expansion has no run of
 * digits with length < 2.  Computed by brute‑force enumeration. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/valid_base11.h"

/* Return the nth term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, guard against caller errors */

    /* Cache of already found valid numbers. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_val = 12;        /* first admissible number */

    /* Maximum search value that fits in seq_t. */
    static const uint64_t max_search =
        (sizeof(seq_t) == 1) ? 0xFFULL :
        (sizeof(seq_t) == 2) ? 0xFFFFULL :
        (sizeof(seq_t) == 4) ? 0xFFFFFFFFULL :
        UINT64_MAX;

    /* Grow cache until it contains the requested term. */
    while (cache_len < n && next_val <= max_search) {
        seq_t cur = (seq_t)next_val;
        if (valid_base11(cur)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cur;
        }
        ++next_val;
    }

    /* If n exceeds the number of valid values that fit in seq_t,
     * any value returned is inappropriate but a further test
     * will not request such n for the current seq_t type. */
    if (cache_len < n) return 0;
    return cache[n - 1];
}
