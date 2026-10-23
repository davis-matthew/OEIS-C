/*
 * A037655  Base 6 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,2.
 * https://oeis.org/A037655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1)*6 + d_n for n>1, where the digit sequence d_n is
 *            3,0,2,3,0,2,... (period 3), a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes A037655 */
seq_t a(idx_t n)
{
    /* cache the already computed terms for O(1) subsequent look‑ups */
    static seq_t *cache     = NULL;   /* array of terms */
    static size_t cache_sz = 0;       /* number of terms stored */
    static size_t cache_cap = 0;      /* allocated capacity */

    if (n < 1)                 /* safely handle out of bounds */
        return (seq_t)0;

    /* Ensure the cache is large enough for the requested index */
    if (cache_sz < (size_t)n) {
        size_t new_cap = (cache_cap > 0) ? cache_cap : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;                    /* grow by factor 2 */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – abort the program (no external interface) */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Pattern of base‑6 digits: 3,0,2 (period 3) */
    static const unsigned pattern[3] = {3, 0, 2};

    /* Initialise the first term if the cache is empty */
    if (cache_sz == 0) {
        cache[0] = (seq_t)pattern[0];   /* a(1) = 3 */
        cache_sz = 1;
    }

    /* Compute missing terms up to index n */
    while (cache_sz < (size_t)n) {
        /* next digit in the pattern */
        unsigned d = pattern[cache_sz % 3];
        /* a(k) = a(k-1)*6 + next_digit  */
        cache[cache_sz] = cache[cache_sz - 1] * (seq_t)6 + (seq_t)d;
        ++cache_sz;
    }

    return cache[n - 1];                /* offset 1 → index n-1 in the array */
}
