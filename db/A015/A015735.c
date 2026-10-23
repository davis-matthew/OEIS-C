/*
 * A015735  Row sums of triangle A004747.
 * https://oeis.org/A015735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Row sums of triangle A004747 (OEIS A015735)
   Recurrence:  a(n) = 6*(n-2)*a(n-1) - (3*n-8)*(3*n-7)*a(n-2) + a(n-3)
   Initial values: a(1)=1, a(2)=3, a(3)=17
   (Indices are 1‑based as in the OEIS entry.) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;

    /* Helper to allocate or grow the cache */
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Initialise base terms if not yet done */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)3;          /* a(2) */
        cache[2] = (seq_t)17;         /* a(3) */
        cache_size = 3;
    }

    /* If the requested term is already cached, return it immediately */
    if (n <= cache_size)
        return cache[n - 1];

    /* Compute terms up to n */
    while (cache_size < n) {
        idx_t idx = cache_size + 1; /* term number to compute (1‑based) */

        /* Ensure there is room for the new term */
        if (cache_size >= cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { /* Allocation failed – fallback to minimal value */
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Use recurrence:
           a(idx) = 6*(idx-2)*a(idx-1) - (3*idx-8)*(3*idx-7)*a(idx-2) + a(idx-3) */
        seq_t coeff1 = (seq_t)6 * (seq_t)(idx - 2);
        seq_t term1   = coeff1 * cache[idx - 2];           /* a(n-1) */
        seq_t coeff2a = (seq_t)(3 * idx - 8);
        seq_t coeff2b = (seq_t)(3 * idx - 7);
        seq_t term2   = (coeff2a * coeff2b) * cache[idx - 3]; /* a(n-2) */
        seq_t term3   = cache[idx - 4];                     /* a(n-3) */
        seq_t next    = term1 - term2 + term3;

        cache[cache_size] = next;
        cache_size++;
    }

    return cache[n - 1];
}
