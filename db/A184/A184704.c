/*
 * A184704  Number of strings of numbers x(i=1..4) in 0..n with sum i*x(i)
 *          equal to n*4.
 * https://oeis.org/A184704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-5) + a(n-8) + a(n-9) - a(n-10); *
 *            Initial values: a(1)=2,a(2)=7,a(3)=14,a(4)=28,a(5)=46,a(6)=74, *
 *            a(7)=107,a(8)=154,a(9)=208,a(10)=278
 * Range:     u8 n <= 9, u16 n <= 67, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle n < 1 – undefined. Return 0 for safety. */
    if (n < 1) return (seq_t)0;

    /* Static cache of computed terms (1‑based). */
    static seq_t *cache = NULL;   /* cache[0] unused */
    static size_t cap   = 0;      /* highest index cached */

    /* Lazy initialization: set up initial 10 terms. */
    if (!cache) {
        cap = 10;
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = 0; /* unused */
        cache[1] = (seq_t)2;   cache[2] = (seq_t)7;
        cache[3] = (seq_t)14;  cache[4] = (seq_t)28;
        cache[5] = (seq_t)46;  cache[6] = (seq_t)74;
        cache[7] = (seq_t)107; cache[8] = (seq_t)154;
        cache[9] = (seq_t)208; cache[10] = (seq_t)278;
    }

    /* Extend cache if needed. */
    if ((size_t)n > cap) {
        size_t newcap = cap;
        while (newcap < (size_t)n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failure */
        /* Compute missing terms using the recurrence. */
        for (size_t i = cap + 1; i <= newcap; ++i) {
            seq_t v = cache[i-1] + cache[i-2];
            v -= (seq_t)2 * cache[i-5];
            v += cache[i-8];
            v += cache[i-9];
            v -= cache[i-10];
            cache[i] = v;
        }
        cap = newcap;
    }

    return cache[(size_t)n];
}
