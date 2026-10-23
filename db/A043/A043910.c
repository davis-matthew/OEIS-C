/*
 * A043910  Numbers k such that 4 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_adjacent_45_54
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A043910: numbers k whose base‑6 representation contains
 *  adjacent 4 and 5 (in either order) but the representation of k+1 does
 *  not. Computed by a simple search with memoisation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adjacent_45_54.h"

/* Return the n‑th element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) return 0;      /* undefined for n < 1, just guard */

    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    if (cache_cap < (size_t)n)
    {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);    /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    seq_t k = (cache_len == 0) ? 0 : cache[cache_len - 1];

    while (cache_len < (size_t)n)
    {
        ++k;
        if (has_adjacent_45_54(k) && !has_adjacent_45_54(k + (seq_t)1))
            cache[cache_len++] = k;
    }

    return cache[(size_t)n - 1];
}
