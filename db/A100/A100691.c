/*
 * A100691  Number of self-avoiding paths with n steps on a triangular lattice
 *          in the strip Z x {0,1}.
 * https://oeis.org/A100691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(2)=12, a(3)=30, a(n)=2*a(n-1)+a(n-3)+6 (n>=4) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t sz = 0;     /* number of values stored */
    static idx_t cap = 0;    /* allocated capacity */

    /* base initial values */
    const seq_t init[4] = {(seq_t)1, (seq_t)4, (seq_t)12, (seq_t)30};

    /* initialize cache on first call */
    if (cache == NULL) {
        /* minimal capacity is 4 */
        cap = (n + 1 >= 4) ? n + 1 : 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory (unlikely) */
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        sz = 4;
    }

    /* grow cache if needed */
    if (n >= sz) {
        if (n + 1 > cap) {
            /* double capacity until it can hold n+1 */
            while (cap < n + 1) {
                cap *= 2;
            }
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* out of memory */
            cache = tmp;
        }
        /* compute new values up to n */
        for (idx_t i = sz; i <= n; ++i) {
            seq_t val;
            if (i < 4) {
                val = init[i];
            } else {
                val = 2 * cache[i - 1] + cache[i - 3] + (seq_t)6;
            }
            cache[i] = val;
        }
        sz = n + 1;
    }

    return cache[n];
}
