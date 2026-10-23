/*
 * A173686  Periodic with period 12: repeat [2,8,2,2,8,5,5,8,5,5,8,2].
 * https://oeis.org/A173686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) - a(n-6) + a(n-9)
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of OEIS A173686.
   Uses iterative computation with memoisation.
   Initial values required by the recurrence:
        a(1)=2, a(2)=8, a(3)=2, a(4)=2, a(5)=8,
        a(6)=5, a(7)=5, a(8)=8, a(9)=5
*/
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* current allocated size          */
    static idx_t len   = 0;   /* number of computed terms (max n) */

    /* invalid indices return 0 */
    if (n < 1)  return 0;

    /* initialise cache on the first call */
    if (cache == NULL) {
        /* start with enough space for the first 9 terms */
        cap = 9;
        if (n > cap) {
            /* be generous enough for the requested n */
            idx_t tmp = cap;
            while (tmp < n) tmp = tmp + tmp/2 + 1;
            cap = tmp;
        }
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return 0;  /* allocation failed */
        /* seed the first 9 terms */
        static const seq_t seed[9] = {2,8,2,2,8,5,5,8,5};
        for (idx_t i = 0; i < 9; ++i) cache[i] = seed[i];
        len = 9;
    }

    /* grow cache if necessary */
    if (n > cap) {
        idx_t new_cap = cap;
        while (new_cap < n) new_cap = new_cap + new_cap/2 + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        /* a(i) = a(i-3) - a(i-6) + a(i-9) */
        /* use addition first to avoid underflow */
        seq_t val = cache[i-4]      /* a(i-3) */
                 + cache[i-10]     /* + a(i-9) */
                 - cache[i-7];     /* - a(i-6)  */
        cache[i-1] = val;           /* store a(i) */
    }
    if (n > len) len = n;
    return cache[n-1];
}
