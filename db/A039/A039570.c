/*
 * A039570  Numbers whose base-5 representation has the same number of 1's,
 *          2's and 4's.
 * https://oeis.org/A039570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth nonnegative integer whose base‑5 representation
 *            contains an identical number of digits 1, 2 and 4 (zero counts
 *            are accepted). For example a(1)=0, a(2)=3, a(3)=15, a(4)=18,
 *            a(5)=39
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   base5_equal_ones_twos_fours
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation maintains a static cache of previously found terms
   so that repeated calls are efficient.  The search scans successive
   integers and tests them for the required property by examining their
   base‑5 digits.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/base5_equal_ones_twos_fours.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined; return 0 for safety */
        return (seq_t)0;

    /* Static cache to remember previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache array */
    static size_t cache_size = 0;  /* number of terms stored */
    static seq_t current_val = 0;  /* next integer to test */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        if (base5_equal_ones_twos_fours(current_val)) {
            /* Expand cache if necessary. */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = current_val;
        }
        current_val++;    /* advance to the next integer */
    }
    return cache[n - 1];
}
