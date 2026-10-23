/*
 * A174392  Neither n-1 nor n+1 is prime.
 * https://oeis.org/A174392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n‑th nonnegative integer k such that neither (k‑1) nor (k+1) is prime.
   The sequence starts 0,5,7,9,11,13,15,17,19,21,23,25,26,27,29,31,33,34,35,37,39,41,43,45…
   The function builds the sequence incrementally and memoises already
   computed terms, so repeated calls with any order of n are O(1) after the
   first.*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *cache = NULL;        /* array of computed terms */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t   cache_len = 0;      /* number of terms stored */

    /* n == 0 is outside the OEIS offset, but return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* initialise the next candidate to search from */
    static idx_t next_candidate = 0;   /* first call sets to 0   */

    /* if we’re still at the start, set next_candidate to 0;
       otherwise start just after the last cached term */
    if (cache_len == 0 && next_candidate == 0)
        next_candidate = 0;
    else if (cache_len > 0 && next_candidate <= cache[cache_len - 1])
        next_candidate = (idx_t)(cache[cache_len - 1] + (seq_t)1);

    /* Grow the cache buffer when needed   */
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Find terms until we have at least n of them  */
    while (cache_len < n) {
        bool ok = true;

        /* Test (cand-1) if it is ≥ 2  */
        if (next_candidate > 1)                /* 1-1 = 0, 0 is nonprime */
            ok = !is_prime(next_candidate - 1);

        /* Test (cand+1) if it is ≥ 2  */
        if (ok && (next_candidate + 1) >= 2)
            ok = !is_prime(next_candidate + 1);

        if (ok) {
            /* store the term and grow the buffer if necessary */
            cache[cache_len++] = (seq_t)next_candidate;
        }

        /* advance to the next integer candidate */
        next_candidate++;
    }

    return cache[n - 1];
}
