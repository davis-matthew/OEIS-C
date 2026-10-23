/*
 * A039319  Numbers whose base-7 representation has the same nonzero number of
 *          2's and 3's.
 * https://oeis.org/A039319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base-7 representation has the same nonzero number
 *            of 2's and 3's
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_equal_2_3_in_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_2_3_in_base7.h"

/* Returns the n-th term of A039319, for n >= 1 */
seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;      /* number of terms stored */
    static uint64_t last_checked = 0;/* largest integer examined so far */

    while (cache_sz < (size_t)n) {
        uint64_t candidate = ++last_checked;
        if (has_equal_2_3_in_base7(candidate)) {
            /* store the candidate in the cache */
            seq_t *tmp = (seq_t*)realloc(cache, (cache_sz + 1) * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failure */
            cache = tmp;
            cache[cache_sz] = (seq_t)candidate;
            ++cache_sz;
        }
    }
    return cache[n - 1];
}
