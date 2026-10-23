/*
 * A155923  Positive numbers y such that y^2 is of the form x^2+(x+17)^2 with
 *          integer x.
 * https://oeis.org/A155923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A155923: a(n) = 6*a(n-3) - a(n-6) for n > 6; 
 * initial values: a(1)=13, a(2)=17, a(3)=25, a(4)=53, a(5)=85, a(6)=137
 */

#include <stdlib.h>  /* malloc, realloc, NULL */

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive indices (behaviour below offset is irrelevant) */
    if (n == 0) return (seq_t)0;

    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* highest index computed (size of cache-1) */

    /* Initialise cache with first 6 terms */
    if (cache == NULL) {
        size_t init_size = (size_t)7;          /* indices 0..6, 0 unused */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)0;   /* unused */
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)25;
        cache[4] = (seq_t)53;
        cache[5] = (seq_t)85;
        cache[6] = (seq_t)137;
        cap = 6;
    }

    /* Grow cache if necessary */
    if ((idx_t)n > (idx_t)cap) {
        size_t new_size = (size_t)(n + 1);      /* indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return cache[cap]; /* use last valid term on failure */
        cache = new_cache;

        /* Compute terms beyond current maximum */
        for (idx_t i = (idx_t)cap + 1; i <= n; ++i) {
            if (i <= 6) continue;  /* initial values already set */
            cache[i] = (seq_t)6 * cache[i - 3] - cache[i - 6];
        }
        cap = (size_t)n;
    }

    return cache[(size_t)n];
}
