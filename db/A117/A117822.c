/*
 * A117822  a(0) = a(1) = 1; for n >= 2, a(n) = a(n-1) + a(n-2) - (n-1) if
 *          that number is positive and not already in the sequence, otherwise
 *          a(n) = a(n-1) + a(n-2) + (n-1).
 * https://oeis.org/A117822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1; for n>=2, a(n)=a(n-1)+a(n-2)-(n-1) if that number
 *            is positive and not already in the sequence, otherwise
 *            a(n)=a(n-1)+a(n-2)+(n-1)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t computed = 0;   /* number of terms stored in cache[0 .. computed-1] */

    /* initialise the base terms only once */
    if (computed == 0) {
        capacity = 2;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        computed = 2;
    }

    size_t N = (size_t)n;          /* use size_t for array indices */

    /* if the requested index is inside the already computed range, just return */
    if (N < computed)
        return cache[N];

    /* grow the cache if necessary */
    if (N + 1 > capacity) {
        capacity = N + 1;
        cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
    }

    /* compute the missing terms up to N */
    for (size_t k = computed; k <= N; ++k) {
        /* compute sum of the two previous terms once */
        seq_t sum = cache[k-1] + cache[k-2];

        /* try the minus candidate first */
        seq_t cand_minus = 0;
        bool use_minus = false;
        seq_t decrement = (seq_t)(k-1);

        /* Ensure we do not underflow; addition in seq_t is defined for the
           types used by the build, and for arbitrarily large integers an
           underflow would yield a huge positive value that is not
           considered valid. */
        if (sum >= decrement) {
            cand_minus = sum - decrement;
            if (cand_minus > 0)  /* zero is not considered positive */
                use_minus = true;
        }

        bool found = false;
        if (use_minus) {
            /* Search all previous elements including the one at index k-1. */
            for (size_t t = 0; t < k; ++t) {
                if (cache[t] == cand_minus) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                cache[k] = cand_minus;
                continue;
            }
        }

        /* otherwise use the plus candidate */
        cache[k] = sum + decrement;
    }

    computed = N + 1;
    return cache[N];
}
