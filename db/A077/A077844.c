/*
 * A077844  Duplicate of A077827.
 * https://oeis.org/A077844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-4), for n >= 4 Initial values: a(0) = 1
 *            a(1) = 3 a(2) = 9 a(3) = 27 This recurrence generates the
 *            sequence 1,3,9,27,79,231,675,1971,... which matches OEIS
 *            sequence A077844 (Duplicate of A077827)
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values of a(n) */
static size_t cache_len = 0;  /* current allocated size (number of elements) */

/* Compute a(n) for arbitrary n >= 0 */
seq_t a(idx_t n)
{
    /* ensure we have room for indices up to n (inclusive) */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure: fall back to minimal computation (should not happen for n <= 10000) */
            /* As a safety, compute iteratively without caching */
            seq_t a0 = 1, a1 = 3, a2 = 9, a3 = 27, ai;
            if (n == 0) return a0;
            if (n == 1) return a1;
            if (n == 2) return a2;
            if (n == 3) return a3;
            for (size_t i = 4; i <= (size_t)n; ++i) {
                ai = a3 * 3 - a0 * 2;
                a0 = a1; a1 = a2; a2 = a3; a3 = ai;
            }
            return a3;
        }
        cache = new_cache;
        /* fill newly allocated portion */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0) cache[i] = 1;
            else if (i == 1) cache[i] = 3;
            else if (i == 2) cache[i] = 9;
            else if (i == 3) cache[i] = 27;
            else /* i >= 4 */ cache[i] = cache[i-1] * 3 - cache[i-4] * 2;
        }
        cache_len = new_len;
    }
    return cache[(size_t)n];
}
