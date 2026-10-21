/*
 * A229136  Number of solutions to Sum_{i=1..n} x_i^2 == 1 (mod 4) with x_i in
 *          0..3.
 * https://oeis.org/A229136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-2) + 2^(2*n-3) (n >= 3) Initial values:
 *            a(1) = 2, a(2) = 8
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* malloc, realloc */
#include "../utils/pow2.h"  /* pow2(idx_t)  -> seq_t */

static seq_t *cache = NULL;   /* cache[0] unused, cache[1] = a(1), ... */
static idx_t  max_cached = 0; /* highest n whose value is stored */

/* compute a(n) for any n >= 1, with memoisation */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, value for n=0 is unused */

    /* initialise cache on first call */
    if (max_cached < 2) {
        /* allocate indices 0..2 inclusive */
        size_t init_size = 3;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        cache[0] = 0;          /* unused */
        cache[1] = (seq_t)2;   /* a(1) */
        cache[2] = (seq_t)8;   /* a(2) */
        max_cached = 2;
    }

    /* extend cache up to requested n if necessary */
    if (n > max_cached) {
        size_t new_size = n + 1;   /* indices 0..n inclusive */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t term = 4 * cache[i - 1];
            term -= 8 * cache[i - 2];
            term += pow2(2 * i - 3);       /* 2^(2*i-3) */
            cache[i] = term;
        }
        max_cached = n;
    }

    return cache[n];
}
