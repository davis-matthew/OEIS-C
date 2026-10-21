/*
 * A061851  Digital representation of n contains only 1's and 0's, is
 *          palindromic and contains no singleton 1's or 0's.
 * https://oeis.org/A061851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 19, u64 n <= 231,
 *            u128 n <= 10000, big any n
 * Helpers:   generate_length, valid_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Constructive enumeration of palindromic decimal strings
   containing only 0 and 1, with no singleton 0's or 1's.
   Terms are generated in order of increasing length, and for
   equal length in increasing numeric value.  This order
   reproduces OEIS A061851. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/generate_length.h"
#include "../utils/valid_runs.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap = 0;
    static idx_t next_len = 2;          /* first admissible length */

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        generate_length(next_len, &cache, &cache_size, &cache_cap);
        ++next_len;
    }

    return cache[n - 1];
}
