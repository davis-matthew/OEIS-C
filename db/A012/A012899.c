/*
 * A012899  E.g.f.: exp(arcsin(x)+log(x+1)).
 * https://oeis.org/A012899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A012899: a(n) = exp(arcsin(x)+log(x+1))
   Recurrence (n ≥ 3):
       3*a(n) = a(n-1) + (3*n^2-15*n+22)*a(n-2) + 2*(n^2-8*n+17)*a(n-3)
   Base values: a(0)=1, a(1)=2, a(2)=3 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0, sz = 0;

    /* initialisation */
    if (cache == NULL) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 2;
        cache[2] = 3;
        sz = 3;
    }

    /* extend cache if needed */
    if ((size_t)n >= sz) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t newcap = cap ? cap * 2 : need;
            if (newcap < need) newcap = need;
            cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cap = newcap;
        }
        /* Compute missing terms up to n */
        for (size_t i = sz; i <= (size_t)n; ++i) {
            if (i < 3) {          /* base values already set */
                continue;
            }
            seq_t i_seq = (seq_t)i;
            seq_t nn = i_seq * i_seq;

            seq_t coeff2 = 3 * nn - 15 * i_seq + (seq_t)22;
            seq_t coeff3 = 2 * (nn - 8 * i_seq + (seq_t)17);

            seq_t term1 = cache[i - 1];
            seq_t term2 = coeff2 * cache[i - 2];
            seq_t term3 = coeff3 * cache[i - 3];

            seq_t sum = term1 + term2 + term3;
            cache[i] = sum / 3;
        }
        sz = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
