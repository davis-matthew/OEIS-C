/*
 * A131804  Antidiagonal sums of triangular array T: T(j,k) = -(k+1)/2 for odd
 *          k, T(j,k) = 0 for k = 0, T(j,k) = j+1-k/2 for even k > 0; 0 <= k
 *          <= j.
 * https://oeis.org/A131804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 3*a(n‑1) – 5*a(n‑2) + 7*a(n‑3) – 7*a(n‑4) * + 5*a(n‑5) –
 *            3*a(n‑6) + a(n‑7) * Initial values: a(0)=0, a(1)=0, a(2)=-1,
 *            a(3)=-1, a(4)=1, * a(5)=2, a(6)=1
 * Range:     i8 n <= 32, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements currently stored */

    /* initial values for indices 0..6 */
    static const seq_t init_vals[7] = {0, 0, -1, -1, 1, 2, 1};

    /* if the requested index is larger than the current cache, grow it */
    if (n >= cache_size) {
        idx_t new_size = n + 1;             /* we need indices 0..n inclusive */

        /* allocate or reallocate memory, casting for C++ compatibility */
        seq_t *new_cache = NULL;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        if (new_cache == NULL) {            /* allocation failed */
            return 0;                       /* undefined, but avoid crash */
        }
        cache = new_cache;

        /* initialise new entries with the fixed initial values if needed */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < 7) {
                cache[i] = init_vals[i];
            }
            /* entries >= 7 will be computed below */
        }

        /* compute missing terms using the recurrence */
        idx_t start = (cache_size <= 7) ? 7 : cache_size;
        for (idx_t i = start; i < new_size; ++i) {
            cache[i] =
                (seq_t)3 * cache[i-1]
              - (seq_t)5 * cache[i-2]
              + (seq_t)7 * cache[i-3]
              - (seq_t)7 * cache[i-4]
              + (seq_t)5 * cache[i-5]
              - (seq_t)3 * cache[i-6]
              +       cache[i-7];
        }

        cache_size = new_size;
    }

    return cache[n];
}
