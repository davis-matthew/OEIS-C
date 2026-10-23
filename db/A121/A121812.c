/*
 * A121812  a(n) = a(n-4)*a(n-3) + a(n-4)*a(n-2) + a(n-3)*a(n-2) +
 *          a(n-4)*a(n-1) + a(n-3)*a(n-1) + a(n-2)*a(n-1).
 * https://oeis.org/A121812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j=2..4} sum_{1<=k<j} a(n-j)*a(n-k) Initial values:
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=1
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 10, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t size = 0;   /* number of terms currently cached   */
    static idx_t cap  = 0;   /* capacity of allocated array         */

    if (n < 1) return (seq_t)0;   /* undefined but harmless */

    /* initialise base values if not yet done */
    if (size < 4) {
        cap = 4;
        cache = (seq_t*)malloc(sizeof(seq_t)*cap);
        cache[0] = (seq_t)0;  /* a(1) */
        cache[1] = (seq_t)1;  /* a(2) */
        cache[2] = (seq_t)1;  /* a(3) */
        cache[3] = (seq_t)1;  /* a(4) */
        size = 4;
    }

    /* if already computed, return immediately */
    if (n <= size)
        return cache[n-1];

    /* grow the cache if we need more space */
    if (n > cap) {
        seq_t *tmp = (seq_t*)malloc(sizeof(seq_t)*n);
        for (idx_t i = 0; i < size; ++i)
            tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cap = n;
    }

    /* compute missing terms up to n */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t a1 = cache[(i-4) - 1];
        seq_t a2 = cache[(i-3) - 1];
        seq_t a3 = cache[(i-2) - 1];
        seq_t a4 = cache[(i-1) - 1];
        seq_t sum = a1*a2 + a1*a3 + a1*a4
                  + a2*a3 + a2*a4
                  + a3*a4;
        cache[i-1] = sum;
    }

    size = n;
    return cache[n-1];
}
