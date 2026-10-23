/*
 * A037256  a(n) = n!*Sum_{i=0..n-1} (n-i)*(-2)^i/(i+1)!.
 * https://oeis.org/A037256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Sum_{i=0..n-1} (n-i)*(-2)^i/(i+1)! Recurrence: a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=10 a(n) = 2*(n-1)*a(n-1) -
 *            (n-4)*(n-1)*a(n-2) - 2*(n-2)*(n-1)*a(n-3) (n>=4) All
 *            computations are performed in the integer type seq_t, which may
 *            be arbitrary precision or a native unsigned integer
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t   cache_len = 0; /* number of allocated slots */
    static idx_t   last     = 0;   /* last index that has a value */

    /*--- first call: initialize cache -------------------------------------*/
    if (cache == NULL) {
        size_t needed = (size_t)(n >= 4 ? n + 1 : 4);   /* 4 slots for 0..3 */
        cache        = (seq_t *)malloc(needed * sizeof(seq_t));
        cache_len    = needed;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)10;       /* a(3) */
        last        = 3;
    }

    /*--- enlarge cache if necessary ---------------------------------------*/
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }

    /*--- compute missing terms up to n -----------------------------------*/
    for (idx_t k = last + 1; k <= n; ++k) {
        /* recurrence only needed for k >= 4 */
        if (k >= 4) {
            seq_t term1 = (seq_t)2 * (seq_t)(k - 1) * cache[k - 1];
            seq_t term2 = (seq_t)(k - 4) * (seq_t)(k - 1) * cache[k - 2];
            seq_t term3 = (seq_t)2 * (seq_t)(k - 2) * (seq_t)(k - 1) * cache[k - 3];
            cache[k] = term1 - term2 - term3;
        } else {
            /* these indices are already initialized (0..3) */
            /* nothing to do */
        }
    }
    last = n;

    return cache[n];
}
