/*
 * A043108  Numbers k such that 0 and 3 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043108: Numbers k such that 0 and 3 occur juxtaposed
   (in any order) in the base‑5 representation of k but not in k‑1.
   The function iteratively generates candidates and stores the
   found values in a static cache so that calls in any order are
   efficient. */

#include <stdlib.h>
#include <stdbool.h>

static bool has_adj_0_3(seq_t x) {
    /* Return true if the base‑5 representation of x contains
       '0' next to '3' (either order). */
    if (x < (seq_t)5)            /* single digit ≥0 */
        return false;
    seq_t prev = x % (seq_t)5;   /* least‑significant digit */
    x /= (seq_t)5;
    while (x > (seq_t)0) {
        seq_t cur = x % (seq_t)5;
        if ((cur == (seq_t)0 && prev == (seq_t)3) ||
            (cur == (seq_t)3 && prev == (seq_t)0))
            return true;
        prev = cur;
        x /= (seq_t)5;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache of found terms.  last_candidate is the largest
       integer tested so far (including those that did not qualify). */
    static seq_t *cache   = NULL;
    static idx_t  cache_len  = 0;
    static idx_t  cache_cap  = 0;
    static seq_t  last_candidate = 0;

    if (n == 0)            /* sequence offset starts at 1 */
        return 0;

    /* Grow cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t k;

        /* Find the next candidate k that satisfies the condition:
           has_adj_0_3(k) && !has_adj_0_3(k-1). */
        do {
            k = ++last_candidate;
        } while (!has_adj_0_3(k) || has_adj_0_3(k - (seq_t)1));

        /* Expand storage if needed. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)            /* allocation failure: abort */
                abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = k;
    }

    return cache[n - 1];
}
