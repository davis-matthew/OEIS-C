/*
 * A039285  Numbers whose base-5 representation has the same nonzero number of
 *          0's and 3's.
 * https://oeis.org/A039285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039285: numbers whose base‑5 representation has
 * the same (non‑zero) number of 0's and 3's.
 *
 * Approach: Brute‑force generation with a static cache.  
 * For each n we extend the cache until it contains the nth term.
 * An element is accepted when the counts of digits 0 and 3 in its base‑5
 * expansion are equal and non‑zero.
 *
 * The cache is grown with seq_realloc (which correctly handles *seq_t*,
 * regardless of whether it is a plain unsigned integer or an arbitrary‑
 * precision class).  No external libraries beyond the standard ones are
 * used, and the code compiles correctly as C11 and as C++17.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* Static cache of the sequence numbers */
static seq_t *seq_cache   = NULL;
static size_t  cache_cap  = 0;
static idx_t   cache_len  = 0;
static seq_t   next_candidate = 0;

/* Ensure that the cache can hold at least one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = seq_realloc(seq_cache, cache_cap, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        seq_cache = tmp;
        cache_cap = new_cap;
    }
}

/* True iff x has an equal, non‑zero number of 0 and 3 digits in base 5. */
static bool is_valid(seq_t x)
{
    seq_t tmp = x;
    int zeros  = 0;
    int threes = 0;

    while (tmp != 0) {
        int d = (int)(tmp % 5);
        if (d == 0) zeros++;
        else if (d == 3) threes++;
        tmp /= 5;
    }
    return (zeros == threes && zeros > 0);
}

/* Required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0 */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        seq_t x = next_candidate;    /* candidate number */
        next_candidate++;            /* advance */

        if (is_valid(x)) {
            ensure_cache_capacity();
            seq_cache[cache_len++] = x;
        }
    }

    return seq_cache[n - 1];
}
