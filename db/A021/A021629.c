/*
 * A021629  Expansion of 1/((1-x)*(1-3*x)*(1-7*x)*(1-12*x)).
 * https://oeis.org/A021629
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

/* 
   Sequence: A021629
   Recurrence: a(0)=1, a(1)=23, a(2)=366, a(3)=5062,
   a(n) = 23*a(n-1) -163*a(n-2) +393*a(n-3) -252*a(n-4)   (n ≥ 5)
*/

#include <stdlib.h> /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Static cache of computed terms, 0‑based indexing. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    size_t old_cap, i;

    /* First call: allocate space for the first 5 terms and store them. */
    if (cache_cap == 0) {
        cache_cap = 5;  /* indices 0..4 are stored */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)366;
        cache[3] = (seq_t)5062;
        cache[4] = (seq_t)65555;
    }

    /* Return early if the requested term is already cached. */
    if ((size_t)n < cache_cap) {
        return cache[(size_t)n];
    }

    /* Expand cache to hold up to index n. */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > cache_cap) {
        /* Preserve the old capacity before reallocation. */
        old_cap = cache_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* Compute new terms from the old capacity up to n. */
        for (i = old_cap; i <= (size_t)n; ++i) {
            /* i ≥ 5, so all indices i-1..i-4 are valid. */
            seq_t term = (seq_t)23 * cache[i - 1]
                      - (seq_t)163 * cache[i - 2]
                      + (seq_t)393 * cache[i - 3]
                      - (seq_t)252 * cache[i - 4];
            cache[i] = term;
        }
    }

    return cache[(size_t)n];
}
