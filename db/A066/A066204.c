/*
 * A066204  a(1) = 4; a(2) = 3; a(3) = 1; for n > 3, a(n) = a(n-1)-(n-1) if
 *          this is positive and has not already appeared in the sequence,
 *          otherwise a(n) = a(n-1)+(n-1).
 * https://oeis.org/A066204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 4, a(2) = 3, a(3) = 1; For n > 3, a(n) = a(n-1)-(n-1) if
 *            this is positive and has not already appeared in the sequence,
 *            otherwise a(n) = a(n-1)+(n-1)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache     = NULL;   /* cached terms, 1‑indexed as cache[n‑1] */
static size_t cache_cap = 0;

/* Ensure the cache is at least `needed` elements long */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range – defensive */

    /* Initialise the cache with the first three terms if necessary */
    if (cache == NULL) {
        ensure_cache_cap(3);
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)1;
    }

    if (n <= 3) return cache[n - 1];

    /* Make room for the requested term */
    ensure_cache_cap((size_t)n);

    /* Compute terms up to n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t prev = cache[i - 2];          /* a(i-1) */
        seq_t cand;
        bool unseen = true;

        /* Only subtract if the result would be positive */
        if (prev > (seq_t)(i - 1)) {
            cand = prev - (seq_t)(i - 1);

            /* Check whether cand already occurred */
            for (idx_t j = 0; j < i - 1; ++j) {
                if (cache[j] == cand) {
                    unseen = false;
                    break;
                }
            }
            if (unseen) {
                cache[i - 1] = cand;        /* accept the subtraction */
                continue;
            }
        }
        /* Either subtraction not allowed or result seen, take addition */
        cache[i - 1] = prev + (seq_t)(i - 1);
    }
    return cache[n - 1];
}
