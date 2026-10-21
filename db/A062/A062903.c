/*
 * A062903  Numbers n such that n and its reversal are both multiples of 13.
 * https://oeis.org/A062903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A062903 – numbers n such that `n` and its decimal
   reversal are both multiples of 13.  The sequence is generated
   lazily, storing found terms in a static cache so that any
   permutation of calls to a(idx_t) works. */

#include <stdlib.h>

/* Forward declaration may appear in the built‑in header; use the
   same linkage as that declaration: no extern "C" here. */
seq_t a(idx_t n)
{
    /* Static cache of found terms.  The cache grows on demand. */
    static seq_t *cache  = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_candidate = 0;       /* next integer to test */

    /* Initialise cache on first call: a(1) = 0. */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t*)realloc(NULL, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;   /* a(1) = 0 */
        cache_len = 1;
        next_candidate = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        idx_t m = next_candidate++;
        if (m % 13 != 0) continue;          /* not a multiple of 13 */

        /* Compute decimal reversal of m. */
        idx_t tmp = m, rev = 0;
        while (tmp > 0) {
            rev = rev * 10 + (tmp % 10);
            tmp /= 10;
        }
        /* For m == 0 the loop above leaves rev == 0, which is fine. */

        if (rev % 13 != 0) continue;        /* reversal not a multiple of 13 */

        /* Store the found number. */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap * 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)m;
    }

    return cache[n - 1];
}
