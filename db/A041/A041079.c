/*
 * A041079  Denominators of continued fraction convergents to sqrt(46).
 * https://oeis.org/A041079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041079 – denominators of continued fraction convergents to sqrt(46).
   The continued‐fraction expansion of sqrt(46) is
       [6; 1,3,1,1,2,6,2,1,1,3,1,12] with period 12.
   Let a0=6 and for n≥1 let a_n be the nth partial quotient:
       a_n = { 1,3,1,1,2,6,2,1,1,3,1,12 } repeated.
   The denominators q_n of the convergents satisfy
       q_0 = 1,  q_1 = 1,
       q_n = a_n * q_{n-1} + q_{n-2}   for n ≥ 2.
   This routine computes q_n for any n ≥ 0 using a memoised array. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = (idx_t)(-1);

    /* Initialise the cache with the first two terms */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                     /* q0 */
        cache[1] = (seq_t)1;                     /* q1 = a1 * q0 + q_{-1} */
        cache_max = 1;
    }

    /* Return if already computed */
    if (n <= cache_max) return cache[n];

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* reallocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Partial quotients of the period (indices 1 … 12) */
    static const idx_t cycle[12] = {1, 3, 1, 1, 2, 6, 2, 1, 1, 3, 1, 12};

    /* Compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        idx_t pos = (i - 1) % 12;                /* index into the period */
        seq_t a_i = (seq_t)cycle[pos];
        cache[i] = a_i * cache[i - 1] + cache[i - 2];
    }

    cache_max = n;
    return cache[n];
}
