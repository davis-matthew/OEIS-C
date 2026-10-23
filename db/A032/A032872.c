/*
 * A032872  Numbers whose base-9 representation Sum_{i=0..m} d(i)*9^i has d(m)
 *          >= d(m-1) <= d(m-2) >= ...
 * https://oeis.org/A032872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   _is_good_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_is_good_base9.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoized sequence values      */
    static size_t cache_cap = 0;     /* allocated capacity             */
    static size_t cache_cnt = 0;     /* number of values cached        */
    static idx_t next_candidate = 1; /* candidate to test next we come across */

    /* generate until we have at least n terms cached */
    while (cache_cnt < n) {
        if (cache_cnt == cache_cap) { /* enlarge cache if needed */
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();         /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        if (_is_good_base9(next_candidate)) {
            cache[cache_cnt++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
