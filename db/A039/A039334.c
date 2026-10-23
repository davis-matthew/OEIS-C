/*
 * A039334  Numbers whose base-8 representation has the same nonzero number of
 *          0's and 6's.
 * https://oeis.org/A039334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8416, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_equal_nonzero_zero_and_six
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: Numbers whose base‑8 representation has the same nonzero number
 *      of 0's and 6's.
 * Formula: a(n) is the n-th positive integer x such that
 *          x in base 8 has an equal and positive number of digits 0 and 6.
 * Offset: a(1) = 48.
 *
 * The routine generates the sequence on demand, caching already found
 * values so that repeated calls are efficient.  The algorithm checks
 * successive integers for the property by examining the base‑8 digits.
 */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_equal_nonzero_zero_and_six.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static seq_t next_candidate = 1;   /* next integer to test */

    /* Grow cache as needed */
    if (n > sz) {
        /* Ensure capacity for new terms */
        if (sz + (n - sz) > cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            while (new_cap < sz + (n - sz))
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();                /* out of memory */
            cache = tmp;
            cap   = new_cap;
        }

        /* Find terms until we have n entries */
        while (sz < n) {
            seq_t val = next_candidate++;
            if (has_equal_nonzero_zero_and_six(val)) {
                cache[sz++] = val;
            }
        }
    }

    return cache[n - 1];
}
