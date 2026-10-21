/*
 * A206263  Number of (n+1) X 5 0..1 arrays with the number of rightwards and
 *          downwards edge increases in each 2 X 2 subblock equal to the
 *          number in all its horizontal and vertical neighbors.
 * https://oeis.org/A206263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A206263: Number of (n+1) X 5 0..1 arrays with the number of
 * rightwards and downwards edge increases in each 2 X 2 subblock equal
 * to the number in all its horizontal and vertical neighbors.
 * 
 * Recurrence for n > 8:
 *   a(n) = 5*a(n-1) - 9*a(n-2) + 5*a(n-3) + 5*a(n-4)
 *           - 9*a(n-5) + 5*a(n-6) - a(n-7)
 * Initial values (1-indexed):
 *   a(1)=50, a(2)=64, a(3)=135, a(4)=260, a(5)=471,
 *   a(6)=800, a(7)=1296, a(8)=2010. */
#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamically allocated cache */
static size_t cache_len = 0;         /* allocated size (>= maximum n+1 seen) */
static size_t computed = 0;          /* highest index that has been computed */

/* The first eight terms (index 0 unused). */
static const seq_t init_terms[9] = {
    0,    /* 0 - unused */
    50,   /* a(1) */
    64,   /* a(2) */
    135,  /* a(3) */
    260,  /* a(4) */
    471,  /* a(5) */
    800,  /* a(6) */
    1296, /* a(7) */
    2010  /* a(8) */
};

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n<1, return 0 as placeholder */

    /* Ensure cache is large enough to hold indices up to n. */
    size_t sz_needed = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)malloc(sz_needed * sizeof(seq_t));
        if (!cache) abort();
        cache_len = sz_needed;
    } else if (cache_len < sz_needed) {
        seq_t *tmp = (seq_t *)realloc(cache, sz_needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = sz_needed;
    }

    /* Compute terms up to n, if not already computed. */
    if ((size_t)n > computed) {
        for (idx_t i = (idx_t)(computed + 1); i <= n; ++i) {
            if (i <= 8) {
                /* Use the explicitly known values. */
                cache[i] = init_terms[i];
            } else {
                /* Recurrence using only additions and subtractions. */
                seq_t pos = 5 * cache[i-1] + 5 * cache[i-3]
                         + 5 * cache[i-4] + 5 * cache[i-6];
                seq_t neg = 9 * cache[i-2] + 9 * cache[i-5] + cache[i-7];
                cache[i] = pos - neg;            /* pos > neg by definition */
            }
        }
        computed = (size_t)n;
    }

    return cache[n];
}
