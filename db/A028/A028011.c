/*
 * A028011  Expansion of 1/((1-2x)(1-7x)(1-9x)(1-12x)).
 * https://oeis.org/A028011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (7*12^(n+3)-25*9^(n+3)+3*7^(n+4)-3*2^(n+3))/1050
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence satisfies the linear recurrence */
/* a(n) = 30*a(n-1) - 311*a(n-2) + 1266*a(n-3) - 1512*a(n-4)   (n ≥ 4) */
/* with initial values a(0)=1, a(1)=30, a(2)=589, a(3)=9606 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation; grows only as needed */
    static seq_t *cache = NULL;       /* pointer to array of seq_t */
    static idx_t cache_size = 0;      /* current size of cache (treat as n+1) */

    /* if requested index not yet computed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                /* need indices 0..n inclusive */
        /* allocate new cache */
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) abort();               /* unable to allocate */

        /* copy old values to new cache */
        for (idx_t i = 0; i < cache_size; ++i) {
            new_cache[i] = cache[i];
        }

        /* initialise first four terms (if needed) */
        if (new_size > 0) new_cache[0] = (seq_t)1;
        if (new_size > 1) new_cache[1] = (seq_t)30;
        if (new_size > 2) new_cache[2] = (seq_t)589;
        if (new_size > 3) new_cache[3] = (seq_t)9606;

        /* compute missing terms using the recurrence */
        idx_t start = 4;                            /* first term to compute */
        if (cache_size > 4) start = cache_size;     /* already computed up to cache_size-1 */

        for (idx_t i = start; i <= n; ++i) {
            seq_t op1 = ((seq_t)30)  * new_cache[i - 1];
            seq_t op2 = ((seq_t)311) * new_cache[i - 2];
            seq_t op3 = ((seq_t)1266) * new_cache[i - 3];
            seq_t op4 = ((seq_t)1512) * new_cache[i - 4];

            /* a(n) = 30*a(n-1) - 311*a(n-2) + 1266*a(n-3) - 1512*a(n-4)  */
            /* Compute as (op1 + op3) - (op2 + op4) to avoid negative intermediate */
            seq_t top    = op1 + op3;
            seq_t bottom = op2 + op4;
            new_cache[i] = top - bottom;
        }

        /* free old cache and update pointers */
        if (cache) free(cache);
        cache      = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
