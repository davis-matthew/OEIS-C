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
 * Validated: standard, pass, 68 terms through n = 786 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a number in base 10 has an equal number of 2's and 3's in its base-7 representation
 * (and at least one of each) */
static bool has_equal_2_3_in_base7(uint64_t x) {
    unsigned int c2 = 0, c3 = 0;
    while (x) {
        uint64_t digit = x % 7ull;
        if (digit == 2ull) ++c2;
        else if (digit == 3ull) ++c3;
        x /= 7ull;
    }
    return (c2 == c3) && (c2 > 0);
}

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
            seq_t *tmp = mem_resize(seq_t, cache, (cache_sz + 1));
            if (!tmp) abort();  /* allocation failure */
            cache = tmp;
            cache[cache_sz] = (seq_t)candidate;
            ++cache_sz;
        }
    }
    return cache[n - 1];
}
