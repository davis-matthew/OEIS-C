/*
 * A079955  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={0,2,3}.
 * https://oeis.org/A079955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 43, u32 n <= 83, u64 n <= 162,
 *            u128 n <= 319, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + a(n-5) + a(n-6) with
   a(0) = 1, a(1) = 0, a(2) = 1, a(3) = 0, a(4) = 1. */

#include <stdlib.h>

/* Cached sequence values */
/* cache_len is the number of filled entries in cache */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Return a(n) for any n >= 0 */
/* Uses memoised recurrence; cache grows as needed. */
seq_t a(idx_t n)
{
    /* Grow cache up to n+1 elements if necessary. */
    if (n >= cache_len) {
        idx_t newlen = n + 1;
        /* C++ requires a cast of the void* result of realloc.
           In C this cast is also allowed. */
        seq_t *newptr = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (!newptr) return 0;          /* memory failure – return zero */
        cache = newptr;

        /* Compute new terms in sequence order. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = 0;
            if (i == 0) {                /* a(0) = 1 */
                val = 1;
            } else {
                if (i >= 2) val += cache[i - 2];
                if (i >= 5) val += cache[i - 5];
                if (i >= 6) val += cache[i - 6];
            }
            cache[i] = val;
        }
        cache_len = newlen;
    }
    return cache[n];
}
