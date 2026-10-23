/*
 * A138803  Imaginary parts of terms (1,1), (2,2) of matrix [1,(1+I); 1,1]^n.
 * https://oeis.org/A138803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 30, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A138803: Imaginary parts of terms (1,1) and (2,2) of matrix [1,(1+I); 1,1]^n
   Recurrence: a(1)=0, a(2)=1, a(3)=3, a(4)=8,
                a(n)=4*a(n-1)-4*a(n-2)-a(n-4)   for n≥5
   Offset: 1   (a(1) is the first term) */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle index below the offset – not required but harmless */
    if (n < 1) return (seq_t)0;

    static seq_t *cache  = NULL;   /* array indexed by the 1‑based index */
    static size_t cache_cap = 0;    /* current allocated capacity (elements) */
    static size_t max_computed = 0; /* highest index already computed */

    size_t idx = (size_t)n;        /* local copy to avoid fragile signed ops */

    /* Ensure the cache is large enough to hold indices up to idx (inclusive) */
    size_t needed_cap = idx + 1;    /* +1 because we keep index 0 unused */
    if (needed_cap < 5) needed_cap = 5;  /* base cases go to index 4   */
    if (cache_cap < needed_cap)
    {
        seq_t *tmp = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed – terminate */
        cache      = tmp;
        cache_cap  = needed_cap;
        /* New cells are undefined until written – no init needed */
    }

    /* Initialise base cases once after allocation */
    if (max_computed < 4)
    {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)8;
        max_computed = 4;
    }

    /* If the requested index has already been computed, return it */
    if (idx <= max_computed)
        return cache[idx];

    /* Compute missing values up to idx using the recurrence */
    for (size_t i = max_computed + 1; i <= idx; ++i)
    {
        seq_t val =
            (seq_t)4 * cache[i - 1] - (seq_t)4 * cache[i - 2] - cache[i - 4];
        cache[i] = val;
    }

    max_computed = idx;
    return cache[idx];
}
