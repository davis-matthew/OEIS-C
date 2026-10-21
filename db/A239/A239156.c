/*
 * A239156  Number of (3+1)X(n+1) 0..3 arrays with no element equal to all
 *          horizontal neighbors or unequal to all vertical neighbors, and new
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A239156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 14*a(n-1) + 14*a(n-2) - 124*a(n-3) - 6*a(n-4) +
 *            90*a(n-5) - 27*a(n-6) * with initial values: * a(1) = 7 * a(2) =
 *            55 * a(3) = 868 * a(4) = 12159 * a(5) = 175471 * a(6) = 2519488
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, NULL */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n) {
    /* static cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of elements currently stored */

    /* initial values for n = 1..6 */
    static const seq_t init_vals[6] = {7, 55, 868, 12159, 175471, 2519488};

    /* Ensure cache has room for up to n elements */
    if (n > cache_size) {
        size_t new_cap = (size_t)n;                       /* new size in seq_t elements */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            abort();                                      /* allocation failure */
        }
        cache = new_cache;

        /* Compute terms from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            idx_t idx = i - 1;                            /* 0‑based array index */
            if (i <= 6) {
                cache[idx] = init_vals[i - 1];
            } else {
                seq_t term = 14 * cache[idx - 1]          /* a(n-1) */
                           + 14 * cache[idx - 2]          /* a(n-2) */
                           + 90 * cache[idx - 5];         /* a(n-5) */
                term -= 124 * cache[idx - 3];           /* a(n-3) */
                term -=   6 * cache[idx - 4];           /* a(n-4) */
                term -=  27 * cache[idx - 6];           /* a(n-6) */
                cache[idx] = term;
            }
        }
        cache_size = n;
    }

    return cache[n - 1];
}
