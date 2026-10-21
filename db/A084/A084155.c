/*
 * A084155  A Pell-related fourth-order recurrence.
 * https://oeis.org/A084155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=4, a(3)=19,
 *            a(n)=8*a(n-1)-18*a(n-2)+8*a(n-3)+7*a(n-4) for n>=4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t last_n = 0;   /* highest index computed so far */
    static size_t cap = 0;     /* allocated capacity of the cache */

    /* Handle the first four indices directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)19;

    /* First use: allocate cache with space for at least n+1 elements
       and initialise the first four values */
    if (cache == NULL) {
        cap = (size_t)(n + 1);
        if (cap < 4) cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)19;
        last_n = 3;
    }

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > cap) {
        cap = (size_t)(n + 1);
        cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_n + 1; i <= n; ++i) {
        seq_t t = (seq_t)7 * cache[(size_t)(i - 4)];
        t += (seq_t)8 * cache[(size_t)(i - 3)];
        t += (seq_t)8 * cache[(size_t)(i - 1)];
        cache[(size_t)i] = t - (seq_t)18 * cache[(size_t)(i - 2)];
    }
    last_n = n;
    return cache[(size_t)n];
}
