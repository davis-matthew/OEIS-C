/*
 * A240359  Number of n X 2 0..3 arrays with no element equal to one plus the
 *          sum of elements to its left or two plus the sum of elements above
 *          it or two plus the sum of the elements diagonally to its
 *          northwest, modulo 4.
 * https://oeis.org/A240359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 9‑term linear recurrence * a(n) = 9*a(n-1) -
 *            36*a(n-2) + 84*a(n-3) - 126*a(n-4) + 126*a(n-5) * - 84*a(n-6) +
 *            36*a(n-7) - 9*a(n-8) + a(n-9) for n > 13. * Initial values:
 *            a(1)=4, a(2)=15, a(3)=43, a(4)=111, a(5)=261, * a(6)=571,
 *            a(7)=1171, a(8)=2278, a(9)=4235, * a(10)=7570, a(11...
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 61, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms (1‑based index) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t next  = 0;   /* next index to compute (i.e., size of cache valid part + 1) */

    /* initialise cache with known initial terms on first call */
    if (!cache) {
        cap  = 16;                     /* start with a modest capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure – unlikely in tests */

        cache[1] = (seq_t)4;
        cache[2] = (seq_t)15;
        cache[3] = (seq_t)43;
        cache[4] = (seq_t)111;
        cache[5] = (seq_t)261;
        cache[6] = (seq_t)571;
        cache[7] = (seq_t)1171;
        cache[8] = (seq_t)2278;
        cache[9] = (seq_t)4235;
        cache[10] = (seq_t)7570;
        cache[11] = (seq_t)13076;
        cache[12] = (seq_t)21918;
        cache[13] = (seq_t)35774;
        next = 14;                     /* next term to compute is a(14) */
    }

    /* if requested n is already computed, return it */
    if (n < next)
        return cache[n];

    /* ensure the underlying array is large enough */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* reallocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute successive terms using the recurrence */
    for (idx_t i = next; i <= n; ++i) {
        /* positive part of the recurrence */
        seq_t pos = cache[i-1] * (seq_t)9
                  + cache[i-3] * (seq_t)84
                  + cache[i-5] * (seq_t)126
                  + cache[i-7] * (seq_t)36
                  + cache[i-9] * (seq_t)1;

        /* negative part of the recurrence */
        seq_t neg = cache[i-2] * (seq_t)36
                  + cache[i-4] * (seq_t)126
                  + cache[i-6] * (seq_t)84
                  + cache[i-8] * (seq_t)9;

        cache[i] = pos - neg;  /* subtraction is safe – pos ≥ neg for all n */
    }
    next = n + 1;         /* a(n) is now computed */

    return cache[n];
}
