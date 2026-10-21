/*
 * A068802  Smaller of two consecutive squares which have no common digits.
 * https://oeis.org/A068802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   append_candidate, digit_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A068802: Smaller of two consecutive squares which have no common digits. */
/* The function searches successive integer pairs (i,i+1), computes i² and (i+1)²,
   and records those squares whose digit sets are disjoint.  Results are cached
   so that calls with any index order are efficient. */

#include <stdlib.h>
#include "../utils/append_candidate.h"
#include "../utils/digit_mask.h"

/* Static state: the cache of found squares, the number found and the
   capacity, and the next i to test. */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static seq_t  *cache     = NULL;
static idx_t  next_i     = 0;   /* next integer i to test */

seq_t a(idx_t n)
{
    /* Fill the cache until it contains n+1 terms. */
    while (cache_len <= n) {
        seq_t sq   = ((seq_t)next_i) * ((seq_t)next_i);
        seq_t sqn1 = ((seq_t)(next_i + 1)) * ((seq_t)(next_i + 1));
        if ((digit_mask(sq) & digit_mask(sqn1)) == 0) {
            append_candidate(&cache, &cache_len, &cache_cap, sq);
        }
        next_i++;
    }
    return cache[n];
}
