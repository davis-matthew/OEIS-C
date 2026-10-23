/*
 * A124353  Number of (directed) Hamiltonian circuits on the n-antiprism
 *          graph.
 * https://oeis.org/A124353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + a(n-5). Initial values:
 *            a(1)=6, a(2)=18, a(3)=32, a(4)=58, a(5)=112
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 57, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n<1 the sequence is undefined; return 0. */
    if (n == 0)
        return (seq_t)0;

    /* 1‑based cache: cache[0] = a(1). */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* current number of cached elements */

    if ((size_t)n > cap) {
        /* Grow the cache to hold n terms. */
        size_t newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)            /* allocation failure → give up */
            return (seq_t)0;
        cache = tmp;

        /* Compute all new terms up to n. */
        for (size_t i = cap; i < newcap; ++i) {
            if (i < 5) {   /* base values */
                switch (i) {
                case 0: cache[i] = 6;   break;
                case 1: cache[i] = 18;  break;
                case 2: cache[i] = 32;  break;
                case 3: cache[i] = 58;  break;
                case 4: cache[i] = 112; break;
                }
            } else {        /* recurrence for i >= 5  → n = i+1 >= 6 */
                cache[i] = 3 * cache[i-1] - cache[i-2] - 2 * cache[i-3] + cache[i-5];
            }
        }
        cap = newcap;
    }

    /* Return the requested term. */
    return cache[(size_t)n - 1];
}
