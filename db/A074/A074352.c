/*
 * A074352  Coefficient of q^1 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(1,2).
 * https://oeis.org/A074352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A074352:  coefficient of q¹ in ν(n)
   Recurrence (from OEIS):  a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) - 4*a(n-4)  for n > 4
   Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=8
   The function caches computed values for arbitrary order calls. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        /* initialise cache with known initial terms */
        cache_size = 5;                    /* indices 0..4 */
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_size);
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 2;
        cache[4] = 8;
    }

    /* extend cache up to requested n */
    if ((idx_t)n + 1 > cache_size) {
        idx_t i;
        idx_t new_size = (idx_t)n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        /* compute missing entries */
        for (i = cache_size; i <= n; ++i) {
            if (i <= 4) {
                /* already set */
                continue;
            }
            seq_t t = 2 * cache[i - 1] + 3 * cache[i - 2];
            t -= 4 * cache[i - 3];
            t -= 4 * cache[i - 4];
            cache[i] = t;
        }
        cache_size = new_size;
    }

    return cache[n];
}
