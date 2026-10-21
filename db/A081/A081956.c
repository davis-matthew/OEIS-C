/*
 * A081956  Duplicate of A056182.
 * https://oeis.org/A081956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 3*a(n-1) + 2^n, a(1) = 2
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 18, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of terms cached.
   We store the terms in a static array so that non‑monotonic
   calls to a() are fast.  For each new index we maintain the
   power 2^n that is needed for the recurrence. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *cache = NULL;    /* array of computed terms */
    static size_t cache_size = 0;  /* how many terms stored (cache_size >= n ⇒ n is cached) */
    static size_t cache_cap  = 0;  /* allocated capacity */
    static seq_t pow2 = 0;        /* 2^current_index when expanding */

    /* initialise cache on first call (a(1)=2) */
    if (cache == NULL) {
        cache_cap   = 128;
        cache_size  = 1;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed – return 0 */
        cache[0] = (seq_t)2;                  /* a(1) */
        pow2     = (seq_t)4;                  /* 2^2 for next computation */
    }

    /* extend cache up to n */
    while (cache_size < n) {
        idx_t idx = cache_size + 1;          /* index of new term: 2,3, … */
        seq_t next = (seq_t)3 * cache[cache_size - 1] + pow2;
        cache[cache_size] = next;
        cache_size++;

        /* grow array if necessary */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* allocation failed – return 0 */
            cache = tmp;
        }

        /* prepare pow2 for next index: 2^idx+1 */
        pow2 = pow2 * (seq_t)2;
    }

    return cache[n - 1];   /* array is 0‑based, a(1) stored at index 0 */
}
