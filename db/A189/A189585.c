/*
 * A189585  Number of permutations of 1..n with displacements restricted to
 *          {-4,-3,-1,0,2}.
 * https://oeis.org/A189585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + a(n-4) + 4*a(n-5) + 2*a(n-6) + a(n-8)
 *            - 4*a(n-9) - 2*a(n-10) - a(n-11) - a(n-12) + a(n-15) with
 *            a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=10, a(6)=20, a(7)=37,
 *            a(8)=72, a(9)=140, a(10)=277, a(11)=553, a(12)=1085, a(13)=2129,
 *            a(14)=4185, a(15)=8228, a(16)=16208, a(17)=31902, …
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */
#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache to memoise computed values */
    static seq_t *cache    = NULL;  /* array indexed from 0.. */
    static size_t  cap     = 0;     /* allocated size of cache   */
    static idx_t   upto    = 0;     /* highest index computed    */

    /* allocate or grow cache to satisfy index n (1‑based) */
    if (n >= (idx_t)cap) {
        size_t new_cap = (size_t)n + 64;          /* extra space for future growth   */
        new_cap *= 2;                            /* double to amortise reallocs      */
        if (new_cap < 16) new_cap = 16;           /* at least enough for base values */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();                 /* out of memory – terminate */
        cache = new_cache;
        cap   = new_cap;
    }

    /* initialise the first 15 values (base cases) */
    if (upto == 0) {
        cache[0] = 0;                 /* unused – keeps indexing simple   */
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 4;
        cache[5] = 10;
        cache[6] = 20;
        cache[7] = 37;
        cache[8] = 72;
        cache[9] = 140;
        cache[10] = 277;
        cache[11] = 553;
        cache[12] = 1085;
        cache[13] = 2129;
        cache[14] = 4185;
        cache[15] = 8228;
        upto = 15;
    }

    /* if already computed, return immediately */
    if (n <= upto) return cache[n];

    /* compute successive terms up to n using the recurrence */
    const seq_t two  = (seq_t)2;
    const seq_t four = (seq_t)4;

    for (idx_t i = upto + 1; i <= n; ++i) {
        seq_t val = cache[i-1] + two*cache[i-3] + cache[i-4]
                  + four*cache[i-5] + two*cache[i-6] + cache[i-8]
                  - four*cache[i-9] - two*cache[i-10]
                  - cache[i-11] - cache[i-12] + cache[i-15];
        cache[i] = val;
    }
    upto = n;

    return cache[n];
}
