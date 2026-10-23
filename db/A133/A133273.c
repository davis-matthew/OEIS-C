/*
 * A133273  Indices of centered decagonal numbers which are also decagonal
 *          numbers.
 * https://oeis.org/A133273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2) - 8; a(1)=1, a(2)=10
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS sequence A133273.
   The function caches previously computed terms to handle out-of-order
   calls and keeps an O(n) generation cost. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    /* Initialise cache on first call */
    if (cache == NULL) {
        size_t init_cap = 3;       /* index from 1, so we need at least 3 slots */
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!cache)
            return 0;             /* allocation failure */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)10;
        max_n = 2;
    }

    /* Grow cache and compute missing terms if necessary */
    if (n > max_n) {
        size_t new_cap = (size_t)n + 1;  /* allocate up to index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                      /* allocation failure */
        cache = tmp;

        for (idx_t i = max_n + 1; i <= n; ++i) {
            cache[i] =
                ((seq_t)18 * cache[i-1]) - cache[i-2] - (seq_t)8;
        }
        max_n = n;
    }

    return cache[n];
}
