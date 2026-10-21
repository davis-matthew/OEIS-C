/*
 * A218759  Unmatched value maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          vertical neighbor in a random 0..1 n X 2 array.
 * https://oeis.org/A218759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 3*a(n-3) + 2*a(n-5) + 2*a(n-6) -
 *            2*a(n-7) valid for n ≥ 8, with initial values a(1)=2, a(2)=6,
 *            a(3)=18, a(4)=47, a(5)=134, a(6)=373, a(7)=1035
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_cache = 0;   /* largest n already computed */
    static size_t cap = 0;        /* allocated size (in elements) */

    /* initial known values (index 0 unused) */
    static const seq_t init[8] = {0, 2, 6, 18, 47, 134, 373, 1035};

    if (n == 0)         /* outside offset – return 0 for safety */
        return 0;

    /* extend cache if necessary */
    if (n > max_cache) {
        /* compute new capacity */
        size_t new_cap = (size_t)(n + 1);   /* indices 0 … n */
        if (new_cap > cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();  /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = max_cache + 1; i <= n; ++i) {
            if (i <= 7) {
                cache[i] = init[i];
            } else {
                cache[i] =
                    2*cache[i-1] + cache[i-2] +                   /* a(n-1), a(n-2) */
                    3*cache[i-3] +                                /* a(n-3)          */
                    2*cache[i-5] + 2*cache[i-6] - 2*cache[i-7];    /* a(n-5),a(n-6),a(n-7) */
            }
        }
        max_cache = n;
    }
    return cache[n];
}
