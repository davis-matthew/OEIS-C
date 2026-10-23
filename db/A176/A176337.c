/*
 * A176337  a(n) = 1 + (1-2^n)*a(n-1) for n > 0, a(0)=0.
 * https://oeis.org/A176337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 + (1 - 2^n) * a(n-1), a(0) = 0
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoisation so that calls may be in any order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;          /* array storing computed terms */
    static size_t cache_len = 0;         /* allocated array length */
    static idx_t computed_upto = 0;      /* highest index that has been computed */
    static seq_t pow2_next = (seq_t)2;   /* 2^(computed_upto+1), used for the next term */

    /* Ensure the cache can hold terms up to n, allocating on first use */
    if (n >= cache_len) {
        size_t new_len = n + 1;
        if (new_len < cache_len * 2) new_len = cache_len * 2;
        if (new_len == 0) new_len = 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        cache_len = new_len;
    }

    /* If the requested index is not yet computed, compute all missing terms */
    if (n > computed_upto) {
        /* The first new term to compute has index computed_upto + 1 */
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];                     /* a(i-1) */
            seq_t term = (seq_t)1 + ( (seq_t)1 - pow2_next ) * prev; /* a(i) */
            cache[i] = term;
            pow2_next <<= 1;                               /* prepare 2^(i+1) for next iteration */
        }
        computed_upto = n;
    }

    return cache[n];
}
