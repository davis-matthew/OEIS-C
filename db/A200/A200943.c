/*
 * A200943  Number of 0..n arrays x(0..3) of 4 elements with nondecreasing
 *          average value and 0..n occur with instance counts within one of
 *          each other.
 * https://oeis.org/A200943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) - a(n-4) + a(n-5) + 3*a(n-6)
 *            - a(n-7) - 2*a(n-8) + a(n-9) for n > 11 Initial values: a(1)=1,
 *            a(2)=7, a(3)=3, a(4)=15, a(5)=46, a(6)=107, a(7)=213, a(8)=383,
 *            a(9)=636, a(10)=997, a(11)=1492
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*=====================================================================
 *  Sequence A200943: Number of 0..n arrays x(0..3) of 4 elements with
 *  nondecreasing average value and 0..n occur with instance counts
 *  within one of each other.
 *====================================================================*/

#include <stdlib.h>

/* Cache for already computed terms (offset 1).  If seq_cache==NULL,
 * the sequence has not been computed yet.  cache_size is the number
 * of entries stored. */
static seq_t *seq_cache   = NULL;
static idx_t  cache_size  = 0;

/* The first 11 terms (a(1) .. a(11)).  They are used as the seed for
 * the recurrence. */
static const seq_t init_vals[11] = {
    (seq_t)1ULL,  (seq_t)7ULL,  (seq_t)3ULL,  (seq_t)15ULL,
    (seq_t)46ULL, (seq_t)107ULL, (seq_t)213ULL, (seq_t)383ULL,
    (seq_t)636ULL, (seq_t)997ULL, (seq_t)1492ULL
};

seq_t a(idx_t n)
{
    /* Exact match for the first 11 terms – no caching needed. */
    if (n <= 11ULL) {
        return init_vals[(size_t)(n - 1ULL)];
    }

    /* Grow or initialise the cache so that it can hold at least n
     * terms (indices 0 .. n-1 correspond to a(1) .. a(n)). */
    if (seq_cache == NULL) {
        /* First allocation: reserve space for n terms. */
        seq_cache = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        if (!seq_cache) return 0;                 /* allocation failed */
        cache_size = n;
        /* Copy the seed values. */
        for (size_t i = 0; i < 11; ++i) {
            seq_cache[i] = init_vals[i];
        }
    } else if (cache_size < n) {
        /* Grow the existing cache. */
        seq_t *tmp = (seq_t *)realloc(seq_cache,
                                      (size_t)n * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failed */
        seq_cache = tmp;
        cache_size = n;
    }

    /* Compute missing terms if any.  The cache is guaranteed to
     * contain terms up to a(cache_size-1).  We need to compute terms
     * from index 11 (a(12)) up to index n-1 (a(n)).  The recurrence
     * uses only previously computed values. */
    for (idx_t idx = 11; idx < n; ++idx) {     /* idx is 0‑based   */
        /* Positive part of the recurrence. */
        seq_t pos = 2ULL * seq_cache[idx - 1]   /* 2*a(idx)   */
                   + seq_cache[idx - 2]          /* + a(idx-1) */
                   + seq_cache[idx - 5]          /* + a(idx-4) */
                   + 3ULL * seq_cache[idx - 6]  /* + 3*a(idx-5) */
                   + seq_cache[idx - 9];         /* + a(idx-8) */

        /* Negative part of the recurrence. */
        seq_t neg = 3ULL * seq_cache[idx - 3]   /* 3*a(idx-2) */
                   + seq_cache[idx - 4]          /* + a(idx-3) */
                   + seq_cache[idx - 7]          /* + a(idx-6) */
                   + 2ULL * seq_cache[idx - 8]; /* + 2*a(idx-7) */

        seq_cache[idx] = pos - neg;
    }

    return seq_cache[(size_t)(n - 1ULL)];
}
