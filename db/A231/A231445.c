/*
 * A231445  Number of (n+1) X (2+1) 0..3 arrays with no element unequal to a
 *          strict majority of its horizontal and antidiagonal neighbors, with
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A231445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) - 133*a(n-2) + 461*a(n-3) - 867*a(n-4) + *
 *            881*a(n-5) - 468*a(n-6) + 108*a(n-7) (empirical recurrence), *
 *            with initial values * a(1)=3, a(2)=15, a(3)=97, a(4)=715, *
 *            a(5)=5643, a(6)=46075, a(7)=382341
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored (a(1) at cache[0]) */
    static const seq_t init[7] = {3, 15, 97, 715, 5643, 46075, 382341};

    if (n == 0) return 0; /* undefined for n<1, but safe return */

    /* Grow cache if needed */
    if (n > cache_size) {
        size_t new_size = (size_t)n;          /* new capacity in elements */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {             /* allocation failure */
            return 0;
        }
        cache = new_cache;
        /* Compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 7) {
                /* Initialise first seven terms */
                cache[i - 1] = init[i - 1];
            } else {
                /* Recurrence uses terms a(i-1) … a(i-7) :
                 * cache indices are zero‑based: a(k) == cache[k-1]   */
                cache[i - 1] =
                      (seq_t)19  * cache[i - 2]
                    - (seq_t)133 * cache[i - 3]
                    + (seq_t)461 * cache[i - 4]
                    - (seq_t)867 * cache[i - 5]
                    + (seq_t)881 * cache[i - 6]
                    - (seq_t)468 * cache[i - 7]
                    + (seq_t)108 * cache[i - 8];
            }
        }
        cache_size = n;   /* update stored size */
    }

    return cache[n - 1];
}
