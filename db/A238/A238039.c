/*
 * A238039  Number of (n+1) X (1+1) 0..2 arrays with the upper median minus
 *          the lower median of every 2 X 2 subblock differing from its
 *          horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A238039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A238039: a(n) = 8*a(n-2) + 152*a(n-4) + 472*a(n-6) + 384*a(n-8)
   (valid for n >= 10).
   Initial values:
     a(1)=81, a(2)=340, a(3)=1440, a(4)=6064, a(5)=25948,
     a(6)=109200, a(7)=466296, a(8)=1962720, a(9)=8385248
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache of computed terms.  Index 0 is unused for 1‑based access. */
    static idx_t cache_size = 0;  /* number of terms stored (excluding dummy 0) */
    static seq_t *cache = NULL;

    /* Base values for 1 <= n <= 9 */
    static const seq_t base[10] = {
        0ULL,          /* dummy for 0th index */
        81ULL,         /* a(1) */
        340ULL,        /* a(2) */
        1440ULL,       /* a(3) */
        6064ULL,       /* a(4) */
        25948ULL,      /* a(5) */
        109200ULL,     /* a(6) */
        466296ULL,     /* a(7) */
        1962720ULL,    /* a(8) */
        8385248ULL     /* a(9) */
    };

    /* Ensure the cache is large enough to hold the requested index. */
    if (cache_size < 9) {
        /* First time: allocate capacity for at least 9 terms + dummy 0. */
        cache_size = 9;
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t));
        for (idx_t i = 1; i <= cache_size; ++i) {
            cache[i] = base[i];
        }
    }
    if (n > cache_size) {
        /* Grow cache to n+1 elements (including dummy 0). */
        size_t new_size = (size_t)n + 1;      /* cast to size_t for malloc */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: abort to keep contract simple. */
            abort();
        }
        cache = tmp;
        /* Compute new terms using the recurrence. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] =
                (seq_t)8 * cache[i-2] +
                (seq_t)152 * cache[i-4] +
                (seq_t)472 * cache[i-6] +
                (seq_t)384 * cache[i-8];
        }
        cache_size = n;
    }
    return cache[n];
}
