/*
 * A145404  Number of perfect matchings (or domino tilings) in O_6 X P_n.
 * https://oeis.org/A145404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) + 47*a(n-2) - 8*a(n-3) - 47*a(n-4) + 12*a(n-5)
 *            + a(n-6) Initial values: a(1)=8, a(2)=137, a(3)=2016,
 *            a(4)=30521, a(5)=459544, a(6)=6926545
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, NULL */

static const seq_t init[7] = {
    0,          /* unused */
    (seq_t)8,       /* a(1) */
    (seq_t)137,     /* a(2) */
    (seq_t)2016,    /* a(3) */
    (seq_t)30521,   /* a(4) */
    (seq_t)459544,  /* a(5) */
    (seq_t)6926545  /* a(6) */
};

static seq_t *cache = NULL;   /* cache[0..last] holds a(0..last) (a(0)=0) */
static size_t  cap   = 0;      /* allocated capacity of cache */
static idx_t   last  = 0;      /* highest index fully computed */

/* Return a(n) for n>=1. a(0) is out of the offset; return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* For indices within the initial block just return the constant. */
    if (n <= 6) return init[n];

    /* Allocate and initialise cache on first call or when undersized. */
    if (cache == NULL) {
        /* Ensure space for indices 0..n (including a(6)). */
        cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();  /* out of memory */
        cache[0] = (seq_t)0;
        for (idx_t i = 1; i <= 6; ++i) cache[i] = init[i];
        last = 6;
    } else if (n >= cap) {
        /* Grow cache to accommodate n. */
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) abort();  /* out of memory */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Follow the recurrence exactly. */
        cache[i] = 12 * cache[i-1] + 47 * cache[i-2]
                 - 8  * cache[i-3] - 47 * cache[i-4]
                 + 12 * cache[i-5] + cache[i-6];
    }
    last = n;

    return cache[n];
}
