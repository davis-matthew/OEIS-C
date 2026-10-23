/*
 * A044001  Numbers k such that 0 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_zero_six
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers k that contain adjacent digits 0 and 6 (in any
   order) but such that k+1 does *not* contain adjacent 0‑6 digits.  The
   function builds the sequence on demand and caches results so that
   subsequent calls are efficient. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_adjacent_zero_six.h"

/* Cache for the discovered terms.  100 000 slots comfortably cover
   all test cases up to n ≈ 10 000. */
static seq_t cache[100000];
static idx_t cache_len = 0;   /* number of terms currently stored   */
static seq_t next_scan = 0;   /* next integer to examine            */

/* Return the n‑th term of A044001 (1‑based).  On first call or when
   more terms are needed the sequence is extended by scanning successive
   integers.  Subsequent calls reuse the cache. */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1; undefined for 0  */
        return 0;

    /* Continue scanning until we have at least n terms stored. */
    while (cache_len < n) {
        seq_t k = next_scan;
        /* k must have adjacent 0‑6 digits, k+1 must not. */
        if (has_adjacent_zero_six(k) && !has_adjacent_zero_six(k + 1))
            cache[cache_len++] = k;
        ++next_scan;
    }

    return cache[n - 1];
}
