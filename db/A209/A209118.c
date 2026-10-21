/*
 * A209118  Number of 6-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and avoiding the patterns z z+1 z+2 and z
 *          z-1 z-2.
 * https://oeis.org/A209118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 67, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A209118
   Formula: a(n) = number of 6‑bead necklaces labeled with numbers –n..n
            not allowing reversal, with sum zero and avoiding the
            patterns z z+1 z+2 and z z-1 z-2.
   Recurrence (for n ≥ 14):
      a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 5*a(n-4) + 7*a(n-5)
           - 8*a(n-6) + 7*a(n-7) - 5*a(n-8) + 5*a(n-9) - 6*a(n-10)
           + 4*a(n-11) - a(n-12)
   Initial values:
      a(1)=15,a(2)=238,a(3)=1373,a(4)=5032,a(5)=14037,a(6)=32802,
      a(7)=67681,a(8)=127310,a(9)=222965,a(10)=368920,a(11)=582789,
      a(12)=885874,a(13)=1303543
*/
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;   /* cache[i] holds a(i) */
    static idx_t cache_used = 0;  /* largest index for which cache[i] is valid */

    /* initial values (1-indexed; cache[0] unused) */
    static const seq_t init_vals[14] = {
        0,             /* dummy for index 0 */
        15, 238, 1373, 5032, 14037, 32802, 67681, 127310,
        222965, 368920, 582789, 885874, 1303543
    };

    /* allocate cache on first call or enlarge if necessary */
    if (cache == NULL) {
        /* need space for indices 0..13 at least */
        cache_used = 13;
        cache = (seq_t*)malloc((cache_used + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* unlikely but guard */
        for (idx_t i = 1; i <= 13; ++i)
            cache[i] = init_vals[i];
    }

    /* if requested index is already computed, return it directly */
    if (n <= cache_used)
        return cache[n];

    /* enlarge cache to hold up to index n */
    if (n > cache_used) {
        size_t old_len = cache_used + 1;
        size_t new_len = n + 1;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* guard */
        /* zero-initialize newly allocated part */
        for (idx_t i = old_len; i <= n; ++i)
            cache[i] = 0;
        /* compute missing terms using recurrence */
        for (idx_t i = cache_used + 1; i <= n; ++i) {
            seq_t sum_pos = 4 * cache[i-1]
                          + 5 * cache[i-3]
                          + 7 * cache[i-5]
                          + 7 * cache[i-7]
                          + 5 * cache[i-9]
                          + 4 * cache[i-11];
            seq_t sum_neg = 6 * cache[i-2]
                          + 5 * cache[i-4]
                          + 8 * cache[i-6]
                          + 5 * cache[i-8]
                          + 6 * cache[i-10]
                          +      cache[i-12];
            cache[i] = sum_pos - sum_neg;   /* safe: sum_pos ≥ sum_neg */
        }
        cache_used = n;
    }

    return cache[n];
}
