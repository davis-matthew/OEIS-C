/*
 * A019742  Expansion of 1/((1-4x)(1-10x)(1-11x)).
 * https://oeis.org/A019742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 25, a(2) = 431;
   for n >= 3:  a(n) = 21*a(n-1) - 110*a(n-2) + 4^n
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* terms already computed: cache[0..max_cached] */
    static idx_t  max_cached = 0;   /* largest index computed so far */
    static seq_t  pow4_val = (seq_t)1; /* 4^max_cached */

    if (cache == NULL) { /* first call – initialise the cache */
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – unlikely in test harness */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)431;
        max_cached = 2;

        /* establish pow4_val = 4^max_cached = 4^2 = 16 */
        pow4_val = (seq_t)1;
        for (idx_t i = 1; i <= max_cached; ++i)
            pow4_val = pow4_val * (seq_t)4;
    }

    /* Return immediately if the requested term is already cached */
    if (n <= max_cached)
        return cache[n];

    /* Grow the cache array to hold values up to n. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0; /* allocation failure */
    cache = tmp;

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        pow4_val = pow4_val * (seq_t)4; /* now pow4_val == 4^i */

        seq_t term = ((seq_t)21 * cache[i - 1]) -
                     ((seq_t)110 * cache[i - 2]) +
                     pow4_val;
        cache[i] = term;
    }

    max_cached = n;
    return cache[n];
}
