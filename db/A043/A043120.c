/*
 * A043120  Numbers k such that 0 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_adjacent_0_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043120:
   a(n) = the nth integer k such that the base‑6 representation of k
   contains an adjacent pair of digits '0' and '5' (in either order)
   but the base‑6 representation of k‑1 does not.  No simple closed form
   is known, so we generate terms by a sequential search. */

/* The implementation below keeps a static cache for the first several
   terms.  The cache is large enough to contain more than 10000 entries
   (chosen to satisfy the time limits for the tests).  The search uses
   base‑6 digit examination only.  No external helper functions are
   required. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_adjacent_0_5.h"

/* Forward declaration of seq_t and idx_t is supplied by the build. */

/* Static cache for memoised terms.  Size 200 000 ≥ expected 10 000 terms. */
static seq_t cache[200000];
static size_t cache_count = 0;
static seq_t next_k = 1;            /* next candidate integer */

/* Return the n‑th term (n ≥ 1).  The function is safe for any order of
   calls, as it generates terms until the desired index is reached. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for n < 1, but return 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_count < n) {
        bool cur_has  = has_adjacent_0_5(next_k);
        bool prev_has = has_adjacent_0_5(next_k - (seq_t)1);

        if (cur_has && !prev_has)
            cache[cache_count++] = next_k;

        ++next_k;
    }

    return cache[n - 1];
}
