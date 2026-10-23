/*
 * A190027  Number of (n+2) X 5 binary arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A190027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) + a(n-6) -
 *            a(n-7)
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 59, u64 n <= 131, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* for n > 11.  Initial values: a(1)=228, a(2)=79, a(3)=158, a(4)=274, a(5)=362,   */
/* a(6)=430, a(7)=506, a(8)=610, a(9)=790, a(10)=1094, a(11)=1539.               */
#pragma once
#include <stdlib.h>

/* External integer types supplied by the build (idx_t, seq_t).  */
seq_t a(idx_t n)
{
    /* static cache of previously computed values, 1‑based indexing */
    static seq_t *cache = NULL;     /* array of terms */
    static size_t  alloc = 0;       /* allocated size (elements)         */
    static idx_t   computed = 0;    /* highest index computed so far     */

    /* fast return for the first 11 terms (no allocation needed) */
    static const seq_t init[12] = {
        0,      /* unused 0th index */
        228,    /* a(1) */
        79,     /* a(2) */
        158,    /* a(3) */
        274,    /* a(4) */
        362,    /* a(5) */
        430,    /* a(6) */
        506,    /* a(7) */
        610,    /* a(8) */
        790,    /* a(9) */
        1094,   /* a(10) */
        1539    /* a(11) */
    };

    if (n <= 11) {
        /* n is within the initial table */
        return init[(size_t)n];
    }

    /* ensure the cache is allocated at least up to n */
    if (cache == NULL) {
        /* allocate space for indices 0..11 at first call */
        alloc = 12;
        cache = (seq_t *)malloc(alloc * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        for (size_t i = 0; i < alloc; ++i)
            cache[i] = init[i];
        computed = 11;
    }

    /* enlarge cache if required */
    if ((size_t)(n + 1) > alloc) {
        size_t new_alloc = alloc;
        while ((size_t)(n + 1) > new_alloc)
            new_alloc <<= 1;                 /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;     /* allocation failure */
        cache = tmp;
        alloc = new_alloc;
    }

    /* if n already computed, just return it */
    if (n <= computed)
        return cache[(size_t)n];

    /* compute terms incrementally using recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = cache[(size_t)i - 1] * (seq_t)2;
        val -= cache[(size_t)i - 2];
        val += cache[(size_t)i - 3];
        val -= cache[(size_t)i - 4] * (seq_t)2;
        val += cache[(size_t)i - 5];
        val += cache[(size_t)i - 6];
        val -= cache[(size_t)i - 7];
        cache[(size_t)i] = val;
    }
    computed = n;
    return cache[(size_t)n];
}
