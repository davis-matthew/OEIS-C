/*
 * A208561  Number of n X 2 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) – 8*a(n-2) for n>3; initial values: a(1)=2,
 *            a(2)=10, a(3)=68
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for the recurrence */
static seq_t *a_cache = NULL;   /* array of previously computed a(n) */
static size_t a_cache_cap = 0;  /* allocated capacity */
static size_t a_cache_size = 0; /* highest index currently computed (≥3 after init) */

/* compute a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* the sequence is defined for n≥1, but guard against mistaken 0 */
        return 0;
    }

    /* initialise cache with the first three terms on the first call */
    if (a_cache == NULL) {
        a_cache_cap = 4;                         /* at least indices 0..3 usable */
        a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
        if (!a_cache) return 0;                  /* allocation failure – fall back to 0 */
        a_cache[1] = 2;                          /* a(1) */
        a_cache[2] = 10;                         /* a(2) */
        a_cache[3] = 68;                         /* a(3) */
        a_cache_size = 3;                        /* we have computed up to index 3 */
    }

    /* if the requested n is already cached, return it */
    if ((size_t)n <= a_cache_size) {
        return a_cache[(size_t)n];
    }

    /* grow the cache to fit the wanted index */
    size_t needed = (size_t)n;
    if (needed + 1 > a_cache_cap) {              /* +1 for 1-based indexing */
        size_t new_cap = a_cache_cap;
        while (new_cap <= needed + 1) {
            new_cap <<= 1;                       /* double until large enough */
        }
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                      /* allocation failure */
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* extend the cache using the recurrence */
    for (size_t i = a_cache_size + 1; i <= needed; ++i) {
        /* a(i) = 8*a(i-1) – 8*a(i-2) */
        a_cache[i] = ((seq_t)8 * a_cache[i-1]) - ((seq_t)8 * a_cache[i-2]);
    }
    a_cache_size = needed;

    return a_cache[(size_t)n];
}
