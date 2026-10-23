/*
 * A039276  Numbers whose base-3 representation has the same nonzero number of
 *          0's and 2's.
 * https://oeis.org/A039276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base‑3 representation has
 *            an equal, non‑zero number of 0’s and 2’s, i.e. count_0(x)>0,
 *            count_2(x)>0 and count_0(x)==count_2(x)
 * Range:     u8 n <= 51, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;          /* array of results */
    static size_t  cache_len = 0;        /* number of stored terms   */
    static size_t  cache_cap = 0;        /* allocated capacity       */
    static seq_t  next_candidate = 1;    /* next integer to test     */
    static int    initialized = 0;       /* guard for first call     */

    if (!initialized) {
        initialized = 1;
        cache_cap = 128;
        cache     = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
        next_candidate = 1;
    }

    /* Generate new terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        seq_t val = next_candidate;
        unsigned cnt0 = 0, cnt2 = 0;

        /* Count 0’s and 2’s in base‑3 representation of val */
        while (val != 0) {
            unsigned digit = (unsigned)(val % 3);   /* digit ∈ {0,1,2} */
            if (digit == 0) ++cnt0;
            else if (digit == 2) ++cnt2;
            val /= 3;
        }

        if (cnt0 > 0 && cnt2 > 0 && cnt0 == cnt2) {
            /* store this term */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = next_candidate;
        }

        next_candidate++;      /* move to next integer to test */
    }

    return cache[n - 1];
}
