/*
 * A162760  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A162760 – Number of reduced words of length n in the
   Coxeter group on 11 generators.
   Recurrence: a(n) = 10·a(n‑1) – 54·a(n‑3) + 45·a(n‑4)  (n ≥ 4)
   Initial values:
     a(0)=1, a(1)=11, a(2)=110, a(3)=1045, a(4)=9900
   Reference: OEIS A162760
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cap     = 0;     /* allocated capacity */
    static idx_t  lastIdx = (idx_t)-1; /* last computed index */

    /* base values for indices 0..4 */
    static const seq_t init[5] = {1, 11, 110, 1045, 9900};

    /* allocate or grow cache if needed */
    if (n < 5) {  /* n is in the initial range */
        if (!cache || cap < 5) {
            cache = (seq_t *)realloc(cache, 5 * sizeof(seq_t));
            if (!cache) exit(EXIT_FAILURE);
            for (idx_t i = 0; i < 5; ++i) cache[i] = init[i];
            cap = 5;
            lastIdx = 4;
        }
        return cache[n];
    }

    /* ensure capacity for at least n+1 terms */
    if (n + 1 > cap) {
        idx_t newCap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        /* if cache was uninitialized, fill initial values */
        if (cap < 5) {
            for (idx_t i = 0; i < 5; ++i) cache[i] = init[i];
            lastIdx = 4;
        }
        cap = newCap;
    }

    /* compute missing terms if needed */
    for (idx_t i = lastIdx + 1; i <= n; ++i) {
        /* recurrence uses a(i-1), a(i-3), a(i-4), all already computed */
        seq_t val = (seq_t)10 * cache[i - 1];
        val -= (seq_t)54 * cache[i - 3];
        val += (seq_t)45 * cache[i - 4];
        cache[i] = val;
    }
    lastIdx = n;
    return cache[n];
}
