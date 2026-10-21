/*
 * A072465  A Fibonacci-like model in which each pair of rabbits dies after
 *          the birth of their 4th litter: a(n) = a(n-2) + a(n-3) + a(n-4) +
 *          a(n-5).
 * https://oeis.org/A072465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) + a(n-5); Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=3
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache to enable arbitrary‑order calls and O(1) access */
    static seq_t *cache    = NULL;
    static size_t cap      = 0;   /* allocated capacity */
    static size_t computed = 0;   /* how many terms are filled (next free index) */

    if (computed == 0) {
        /* initialize with initial values */
        cap = 16;                       /* preallocated capacity */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = 1;  cache[1] = 1;  cache[2] = 1;
        cache[3] = 2;  cache[4] = 3;
        computed = 5;
    }

    /* return if already computed */
    if ((size_t)n < computed) return cache[n];

    /* enlarge buffer if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – unlikely on small n */
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms iteratively */
    for (size_t i = computed; i <= (size_t)n; ++i)
        cache[i] = cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5];

    computed = (size_t)n + 1;
    return cache[n];
}
