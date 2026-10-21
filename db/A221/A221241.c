/*
 * A221241  Number of n X 3 arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor.
 * https://oeis.org/A221241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 222*a(n-2) + 540*a(n-3) - 324*a(n-4) (n ≥ 5)
 *            Initial values: a(1) = 2, a(2) = 50, a(3) = 1056, a(4) = 21660
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle offset 1; undefined for n == 0, return 0 for safety */
    if (n == 0) return 0;

    /* Static cache: cache[0] = a(1), cache[1] = a(2), ... */
    static seq_t *cache = NULL;   /* cache of computed terms */
    static idx_t   size  = 0;     /* number of terms currently stored (max index) */
    static idx_t   cap   = 0;     /* allocated capacity of cache */

    /* Grow cache if necessary */
    if (n > size) {
        idx_t new_cap = cap;
        if (new_cap < n) new_cap = n;
        if (new_cap == 0) new_cap = 4;   /* initial capacity */

        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failure handling: return 0 */
            return 0;
        }
        cache = new_cache;
        cap   = new_cap;

        /* Compute missing terms up to n */
        for (idx_t i = size + 1; i <= n; ++i) {
            seq_t val;
            switch (i) {
                case 1: val = 2;      break;
                case 2: val = 50;     break;
                case 3: val = 1056;   break;
                case 4: val = 21660;  break;
                default:
                    /* recurrence: a(i) = 30*a(i-1) - 222*a(i-2) + 540*a(i-3) - 324*a(i-4) */
                    val = 30*cache[i-2]  /* a(i-1) */
                        - 222*cache[i-3]/* a(i-2) */
                        + 540*cache[i-4]/* a(i-3) */
                        - 324*cache[i-5];/* a(i-4) */
                    break;
            }
            cache[i-1] = val;  /* store at index i-1 */
        }
        size = n;
    }

    return cache[n-1];
}
