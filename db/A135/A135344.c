/*
 * A135344  a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), with initial values 1,1,1,1.
 * https://oeis.org/A135344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), with a(0)=1, a(1)=1,
 *            a(2)=1, a(3)=1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;
    static idx_t max = (idx_t)-1;   /* uninitialized flag */
    static size_t cap = 0;

    if (max == (idx_t)-1) {             /* first call : initialise */
        cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1; cache[1] = 1; cache[2] = 1; cache[3] = 1;
        max = 3;
    }

    if (n <= max)                       /* value already computed */
        return cache[n];

    /* Grow the cache to hold indices up to n */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap)
            newcap = newcap ? newcap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;             /* out of memory – should not occur */
        cache = tmp;
        cap = newcap;
    }

    for (idx_t i = max + 1; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) + 3*a(i-4) - a(i-3) */
        seq_t val = 3 * cache[i-1] + 3 * cache[i-4] - cache[i-3];
        cache[i] = val;
    }

    max = n;
    return cache[n];
}
