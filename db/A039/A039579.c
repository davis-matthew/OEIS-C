/*
 * A039579  Numbers whose base-6 representation has the same number of 0's,
 *          2's and 5's.
 * https://oeis.org/A039579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n‑th positive integer whose base‑6 representation has the
 *            same number of 0’s, 2’s and 5’s. The function enumerates
 *            consecutive integers and keeps all found terms in a static cache
 * Range:     u8 n <= 45, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"

static seq_t       *cache = NULL;   /* cache of found terms   */
static size_t      cache_len = 0;   /* number of cached terms */
static size_t      cache_cap = 0;   /* capacity of cache      */
static seq_t       cur = 1;         /* next integer to test   */

/* returns true iff v's base‑6 representation contains equal numbers of the
   digits 0, 2 and 5.  Works for arbitrary precision seq_t. */
static bool is_good(seq_t v)
{
    seq_t tmp = v;
    unsigned int cnt0 = 0, cnt2 = 0, cnt5 = 0;
    while (tmp > 0) {
        seq_t dig = tmp % 6;
        unsigned int d = (unsigned int)dig; /* d ∈ {0,…,5} */
        if (d == 0) ++cnt0;
        else if (d == 2) ++cnt2;
        else if (d == 5) ++cnt5;
        tmp = tmp / 6;
    }
    return (cnt0 == cnt2) && (cnt2 == cnt5);
}

/* external linkage as requested */
seq_t a(idx_t n)
{
    /* extend cache until it holds the n‑th term */
    while (cache_len < n) {
        if (is_good(cur))
            append_candidate(&cache, &cache_len, &cache_cap, cur);
        cur = cur + 1;      /* next integer to examine */
    }
    return cache[n - 1];
}
