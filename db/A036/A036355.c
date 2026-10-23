/*
 * A036355  Fibonacci-Pascal triangle read by rows.
 * https://oeis.org/A036355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) is the n‑th entry of the Fibonacci‑Pascal triangle read by
 *            rows. Triangle recurrence for T(n,k): T(n,k) = T(n-1,k-1) +
 *            T(n-2,k-2) + T(n-1,k) + T(n-2,k), with zero outside the bounds
 *            and initial value T(0,0)=1. The sequence is obtained by listing
 *            the rows: a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1...
 * Range:     u8 n <= 37, u16 n <= 96, u32 n <= 333, u64 n <= 1148,
 *            u128 n <= 4316, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute the triangular number r*(r+1)/2 using 128‑bit arithmetic to avoid overflow */
static inline idx_t triangular(idx_t r)
{
    return (idx_t)(((unsigned __int128)r * (r + (idx_t)1)) / (idx_t)2);
}

seq_t a(idx_t n)
{
    /* Static cache of the flattened triangle */
    static seq_t *tri = NULL;              /* flattened triangle values */
    static idx_t cached_rows = 0;           /* highest row computed (0‑based) */
    static idx_t cached_terms = 0;          /* number of elements stored in tri */

    /* Initialise cache if required */
    if (tri == NULL) {
        tri = (seq_t *)malloc(sizeof(seq_t) * 1);
        tri[0] = (seq_t)1;
        cached_rows = 0;
        cached_terms = 1;
    }

    /* Extend the cache until the requested index n is covered */
    while (triangular(cached_rows + (idx_t)1) <= n) {
        idx_t i = cached_rows + (idx_t)1;            /* new row index */
        size_t new_total = (size_t)(cached_terms + (i + (idx_t)1));

        /* Reallocate space for the new row */
        seq_t *tmp = (seq_t *)realloc(tri, new_total * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed; return 0 as a safe fallback */
            return (seq_t)0;
        }
        tri = tmp;

        /* Compute each element of the new row */
        for (idx_t k = 0; k <= i; ++k) {
            seq_t val = 0;

            /* T(i-1, k-1) */
            if (k >= (idx_t)1) {
                val += tri[(size_t)(triangular(i - (idx_t)1) + (k - (idx_t)1))];
            }
            /* T(i-2, k-2) */
            if (i >= (idx_t)2 && k >= (idx_t)2) {
                val += tri[(size_t)(triangular(i - (idx_t)2) + (k - (idx_t)2))];
            }
            /* T(i-1, k) */
            if (k <= i - (idx_t)1) {
                val += tri[(size_t)(triangular(i - (idx_t)1) + k)];
            }
            /* T(i-2, k) */
            if (i >= (idx_t)2 && k <= i - (idx_t)2) {
                val += tri[(size_t)(triangular(i - (idx_t)2) + k)];
            }

            tri[(size_t)(triangular(i) + k)] = val;
        }

        /* Update cache counters */
        cached_terms = new_total;
        cached_rows = i;
    }

    /* Return the requested value */
    return tri[(size_t)n];
}
