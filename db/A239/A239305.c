/*
 * A239305  Expansion of (4*x^4-5*x^3-x^2+3*x-1) /
 *          (2*x^5+3*x^4-4*x^3-3*x^2+4*x-1).
 * https://oeis.org/A239305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - 4*a(n-3) + 3*a(n-4) + 2*a(n-5) for
 *            n>=5 Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=13
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to support arbitrary call order */
    static seq_t *cache = NULL;       /* array of computed values */
    static size_t   cap   = 0;        /* current capacity of cache   */
    static size_t   used  = 0;        /* number of values stored    */

    /* initialise cache with the first 5 terms on the first call */
    if (cap == 0) {
        cap = 5;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE); /* fail gracefully */
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)2;   /* a(2) */
        cache[3] = (seq_t)6;   /* a(3) */
        cache[4] = (seq_t)13;  /* a(4) */
        used = 5;
    }

    /* if the requested value is already cached, return it */
    if ((idx_t)used > n)
        return cache[n];

    /* otherwise extend the cache up to n */
    if ((size_t)n + 1 > cap) {
        /* grow capacity (double or just enough for n+1) */
        size_t new_cap = cap * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = new_cap;
    }

    for (idx_t i = used; i <= n; ++i) {
        /* recurrence:
           a(i) = 4*a(i-1) - 3*a(i-2) - 4*a(i-3) + 3*a(i-4) + 2*a(i-5)   */
        seq_t term1 = ((seq_t)4) * cache[i-1];
        seq_t term2 = ((seq_t)3) * cache[i-2];
        seq_t term3 = ((seq_t)4) * cache[i-3];
        seq_t term4 = ((seq_t)3) * cache[i-4];
        seq_t term5 = ((seq_t)2) * cache[i-5];

        seq_t val = term1 - term2 - term3 + term4 + term5;
        cache[i] = val;
    }
    used = n + 1;
    return cache[n];
}
