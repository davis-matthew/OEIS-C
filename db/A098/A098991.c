/*
 * A098991  Numbers n where A098035(n)=0.
 * https://oeis.org/A098991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 935, u32 n <= 935, u64 n <= 935,
 *            u128 n <= 935, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A098991:  numbers n with  Σ_{d|n} μ(d+1) = 0
   (OEIS notation).  The formula is computed directly without
   storing a b-file; a(1) corresponds to the first element
   of the list. */

#include <stdlib.h>
#include "../utils/mu_of.h"

/* Cache up to a reasonable amount of terms.  For n ≤ 10000
   a cache of 20000 elements is more than enough.  The first
   20000 terms are enough for the time‑limit requirements. */
static seq_t  cache[20000];
static size_t cache_len   = 0;   /* number of terms stored */
static idx_t  examined    = 0;   /* largest integer examined */

/* Computes a(n) where n ≥ 1.  The function is memoised: the
   first call triggers a search for sufficient terms, later
   calls reuse the cached results. */
seq_t a(idx_t n)
{
    /* Build the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Examine the next integer. */
        ++examined;

        /* Compute Σ_{d|examined} μ(d+1). */
        int sum = 0;   /* μ values are -1,0,1 */
        for (idx_t d = 1; d <= examined / d; ++d) {
            if (examined % d == 0) {
                sum += mu_of(d + 1);
                idx_t e = examined / d;
                if (e != d)
                    sum += mu_of(e + 1);
            }
        }

        /* If the sum is zero, we store the number as a term. */
        if (sum == 0) {
            cache[cache_len++] = (seq_t)examined;
        }
    }

    /* a(1) is cache[0], a(n) is cache[n-1]. */
    return cache[n - 1];
}
