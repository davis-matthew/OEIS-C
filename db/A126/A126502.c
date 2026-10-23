/*
 * A126502  Number of base 7 n-digit numbers with adjacent digits differing by
 *          four or less.
 * https://oeis.org/A126502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 6*a(n-1) + 2*a(n-2) - 3*a(n-3) for n > 2; a(0) = 1, a(1)
 *            = 7, a(2) = 43
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the OEIS sequence A126502 (base‑7 n‑digit numbers with adjacent
   digits differing by ≤4).  Results are memoised so calls can be in any
   order.  Works with 8, 16, 32, 64 or 128 bit unsigned types and with an
   arbitrary‑precision seq_t class; all arithmetic stays in seq_t. */
seq_t a(idx_t n)
{
    static seq_t *cache  = (seq_t *)NULL;   /* cached values */
    static idx_t cache_size = 0;           /* number of cached terms */
    static idx_t cache_cap  = 0;           /* allocated capacity */

    /* initialise base values once */
    if (cache_size == 0) {
        cache_cap = 4;                                    /* small start */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)43;
        cache_size = 3;                                   /* 0..2 cached */
    }

    /* if requested term already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* grow capacity if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;                                 /* double */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) { cache[i] = (seq_t)1; continue; }
        if (i == 1) { cache[i] = (seq_t)7; continue; }
        if (i == 2) { cache[i] = (seq_t)43; continue; }

        seq_t term = cache[i-1];
        term *= (seq_t)6;                    /* 6*a(i-1) */
        term += cache[i-2] * (seq_t)2;        /* +2*a(i-2) */
        term -= cache[i-3] * (seq_t)3;        /* -3*a(i-3) */
        cache[i] = term;
    }

    cache_size = n + 1;
    return cache[n];
}
