/*
 * A177881  Partial sums of round(3^n/10).
 * https://oeis.org/A177881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = floor((3*3^n - 1)/20) Recurrence used: a(0)=0, a(1)=0,
 *            a(2)=1, a(3)=4, a(n)=4*a(n-1)-4*a(n-2)+4*a(n-3)-3*a(n-4) for n>3
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed a(i) values */
static idx_t cache_size = 0;     /* current cached size (max index+1) */

static const seq_t init_vals[4] = {0, 0, 1, 4};

seq_t a(idx_t n)
{
    /* Allocate or expand the cache if needed */
    if (cache_size <= n) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *new_cache = NULL;

#ifdef __cplusplus
        /* C++: allocate with constructors */
        new_cache = new seq_t[new_size];
        /* copy old values */
        for (idx_t i = 0; i < old_size; ++i) new_cache[i] = cache[i];
        /* free old array */
        delete[] cache;
#else
        /* C: use malloc/realloc */
        void *p = (old_size == 0) ? malloc(new_size * sizeof(seq_t))
                                  : realloc(cache, new_size * sizeof(seq_t));
        if (!p) abort();  /* allocation failed */
        new_cache = (seq_t*)p;
#endif
        cache = new_cache;
        cache_size = new_size;

        /* Compute any new entries */
        for (idx_t i = old_size; i < new_size; ++i) {
            if (i < 4) {
                cache[i] = init_vals[i];
            } else {
#ifdef __cplusplus
                cache[i] = 4 * cache[i-1]
                          - 4 * cache[i-2]
                          + 4 * cache[i-3]
                          - 3 * cache[i-4];
#else
                /* use 128‑bit intermediate to avoid overflow */
                __int128_t v = (__int128_t)4 * cache[i-1]
                              - (__int128_t)4 * cache[i-2]
                              + (__int128_t)4 * cache[i-3]
                              - (__int128_t)3 * cache[i-4];
                cache[i] = (seq_t)v;
#endif
            }
        }
    }
    return cache[n];
}
