/*
 * A121795  Expansion of -x*(x+1)^2*(x^8-2*x^7+2*x^6-3*x^5+3*x^4+2*x^2-x+1) /
 *          (x^12+6*x^6-1).
 * https://oeis.org/A121795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 39, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 6*a(n-6) + a(n-12), with
 * a(1)=1, a(2)=1, a(3)=1, a(4)=3, a(5)=5, a(6)=3,
 * a(7)=5, a(8)=5, a(9)=5, a(10)=18, a(11)=31, a(12)=18.
 */
#include <stddef.h>
#include <stdlib.h>

static const seq_t base[13] = {
    0,  /* index 0 is unused */
    1, 1, 1, 3, 5, 3, 5, 5, 5, 18, 31, 18
};

#ifdef __cplusplus
static seq_t* cache = nullptr;
static idx_t cache_size = 0;
#else
static seq_t *cache = NULL;
static idx_t cache_size = 0;
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for out‑of‑range calls */

    /* Allocate or grow the cache as needed */
    if (cache == NULL || n + 1 > cache_size) {
        idx_t new_size = n + 1;          /* indices 0 … n */
        seq_t *new_cache;

#ifdef __cplusplus
        new_cache = new seq_t[new_size];
#else
        new_cache = (seq_t*)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_cache) return 0;        /* allocation failed */
#endif
        /* copy old contents if any */
        if (cache != NULL) {
            for (idx_t i = 1; i < cache_size; ++i)
                new_cache[i] = cache[i];
        }

        /* initialize indices that are not yet set */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 12) {
                new_cache[i] = base[i];
            } else {
                /* recurrence: a(n) = 6*a(n-6) + a(n-12) */
                new_cache[i] = 6 * new_cache[i - 6] + new_cache[i - 12];
            }
        }

        /* free old cache */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache      = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
