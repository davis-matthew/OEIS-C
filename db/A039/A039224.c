/*
 * A039224  Numbers whose base-12 representation has the same number of 1's
 *          and 6's.
 * https://oeis.org/A039224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   has_equal_ones_and_sixes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑12 representation has equal
   number of 1's and 6's.  The values are obtained by a linear
   search over successive integers. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_equal_ones_and_sixes.h"

/* Static cache for previously computed terms. */
#define MAX_CACHE 20000          /* sufficient for n ≈ 10000 */
static seq_t cache[MAX_CACHE];
static idx_t cache_size = 0;     /* number of filled entries */

/* Return the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)               /* safety for accidental zero index */
        return (seq_t)0;

    /* Extend cached sequence up to the requested index. */
    for (idx_t i = cache_size; i < n; ++i) {
        seq_t candidate = (i == 0) ? (seq_t)0
                                   : cache[i - 1] + (seq_t)1;
        while (!has_equal_ones_and_sixes(candidate))
            ++candidate;
        cache[i] = candidate;
    }
    cache_size = n;
    return cache[n - 1];
}
