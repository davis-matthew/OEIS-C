/*
 * A233811  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having the sum of the squares of all six edge and diagonal
 *          differences equal to 11.
 * https://oeis.org/A233811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Empirical recurrence for A233811:
   a(n) = 30*a(n-2) - 220*a(n-4) + 240*a(n-6)   for n ≥ 7
   Initial values:
     a(1) = 96,  a(2) = 428,  a(3) = 1824,
     a(4) = 8136, a(5) = 34848, a(6) = 155488
   Sequence is indexed from 1.  */

#include <stddef.h>
#include <stdlib.h>

/* Compute a(n) for OEIS A233811. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache[0] unused; cache[i] = a(i) */
    static idx_t cache_size = 0;         /* size of allocated array */
    static idx_t computed_upto = 0;      /* largest index already computed */

    if (n < 1) return 0;                 /* outside defined range */

    /* Allocate or grow cache if necessary. */
    idx_t needed = n + 1;                /* keep cache[0] unused */
    if (needed > cache_size) {
        cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        cache_size = needed;

        /* Initialise the known initial terms. */
        if (cache_size > 1) cache[1] = 96;
        if (cache_size > 2) cache[2] = 428;
        if (cache_size > 3) cache[3] = 1824;
        if (cache_size > 4) cache[4] = 8136;
        if (cache_size > 5) cache[5] = 34848;
        if (cache_size > 6) cache[6] = 155488;
    }

    /* Compute any missing terms up to n using the recurrence. */
    if (n >= 7) {
        idx_t start = computed_upto + 1;
        if (start < 7) start = 7;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = cache[i-2] * (seq_t)30
                       - cache[i-4] * (seq_t)220
                       + cache[i-6] * (seq_t)240;
        }
    }

    if (n > computed_upto) computed_upto = n;

    return cache[n];
}
