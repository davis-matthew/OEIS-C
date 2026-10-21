/*
 * A055571  Sum of digits of a(n)^5 is greater than or equal to a(n).
 * https://oeis.org/A055571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   big_pow, digit_sum_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A055571: a(n) = the n‑th nonnegative integer a such that
   digit_sum(a^5) ≥ a.  The offset is 1 (so a(1)=0).  The code
   generates each term by brute force and caches the results so that
   multiple calls are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/digit_sum_seq.h"

static seq_t *cache = NULL;          /* cached terms */
static idx_t  cache_len = 0;         /* number of cached terms */
static idx_t  cache_cap = 0;         /* allocated capacity of cache */
static idx_t  next_candidate = 0;    /* next integer to test for property */

/* Ensure that cache contains at least 'target' terms (1‑based count). */
static void
ensure_cache(idx_t target)
{
    /* Grow cache array if necessary.  C++ requires an explicit cast. */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;                     /* allocation failure: stop */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Find new qualifying integers until we have enough terms. */
    while (cache_len < target) {
        seq_t cand = (seq_t) next_candidate;
        seq_t pow5 = big_pow(cand, (seq_t)5);
        seq_t s    = digit_sum_seq(pow5);
        if (s >= cand) {
            cache[cache_len++] = cand;        /* store qualifying integer */
        }
        ++next_candidate;
    }
}

/* Main sequence function.  It is safe for any order of calls. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;                   /* below the offset: undefined */
    ensure_cache(n);                         /* make sure we have n terms */
    return cache[n - 1];
}
