/*
 * A187066  Let i be in {1,2,3} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3} = {-2,0,1}, n=2*r+p_i, and define a(-2)=0. Then,
 *          a(n)=a(2*r+p_i) gives the quantity of H_(7,2,0) tiles in a
 *          subdivided H_(7,i,r) tile after linear scaling by the factor x^r,
 *          where x=sqrt(2*cos(Pi/7)).
 * https://oeis.org/A187066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 41, u32 n <= 79, u64 n <= 154,
 *            u128 n <= 305, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2)+2*a(n-4)-a(n-6);
   Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=1 */

#include <stdlib.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;
static idx_t cache_nmax = 0;   /* largest n for which cache[n] is valid */
static size_t cache_cap   = 0;  /* allocated size */

seq_t a(idx_t n) {
    /* Base values for n = 0..5 */
    static const seq_t base[6] = {1, 0, 0, 1, 2, 1};

    /* Initialize cache if needed */
    if (!cache) {
        cache_cap = 64; /* initial capacity, can grow */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* fallback: cannot allocate */
        /* set base values */
        for (idx_t i = 0; i < 6; ++i) cache[i] = base[i];
        cache_nmax = 5;
    }

    /* If requested n is already cached, return it */
    if (n <= cache_nmax) return cache[n];

    /* Ensure the cache is large enough to hold up to n */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute the sequence up to n using the recurrence */
    for (idx_t i = cache_nmax + 1; i <= n; ++i) {
        seq_t sum = cache[i-2] + ((seq_t)2 * cache[i-4]);
        seq_t val = sum - cache[i-6];
        cache[i] = val;
    }
    cache_nmax = n;
    return cache[n];
}
