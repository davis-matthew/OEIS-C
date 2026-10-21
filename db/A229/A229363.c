/*
 * A229363  a(1) = 0; for n > 1: a(n) = smallest even number greater than
 *          a(n-1) which does not use any digit used by a(n-1).
 * https://oeis.org/A229363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 73, u64 n <= 151,
 *            u128 n <= 304, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n‑2) + 10·a(n‑8) – 10·a(n‑10)  for n > 17,
   with initial values:
   a(1)=0, a(2)=2, a(3)=4, a(4)=6, a(5)=8, a(6)=10,
   a(7)=22, a(8)=30, a(9)=42, a(10)=50, a(11)=62, a(12)=70,
   a(13)=82, a(14)=90, a(15)=112, a(16)=300, a(17)=412. */

#include <stdlib.h>

static const seq_t init_vals[] = {
    0, 2, 4, 6, 8, 10, 22, 30, 42, 50,
    62, 70, 82, 90, 112, 300, 412
};
static const size_t init_len = sizeof(init_vals) / sizeof(init_vals[0]);

static seq_t *cache = NULL;      /* cached sequence values */
static size_t  cache_size = 0;   /* allocated size of cache */
static idx_t   cache_max  = 0;   /* maximum n that has been computed */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n <= init_len) {
        return init_vals[n - 1];
    }

    /* Ensure cache is allocated at least up to 'n' */
    if (cache == NULL) {
        cache_size = n > init_len ? n : init_len;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure; unlikely in tests */
        /* Copy initial values into cache */
        for (size_t i = 0; i < init_len; ++i) {
            cache[i] = init_vals[i];
        }
        /* Remaining entries are zero-initialized implicitly */
    } else if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return 0; /* reallocation failure */
        cache = tmp;
        cache_size = n;
        /* No need to initialize new entries explicitly; they
           will be filled in the loop below. */
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= init_len) {
            /* This should already be filled, but keep for safety */
            cache[i - 1] = init_vals[i - 1];
        } else {
            /* Recurrence: a(i) = a(i-2) + 10*a(i-8) - 10*a(i-10) */
            size_t idx1 = (size_t)(i - 3);  /* i-2 */
            size_t idx2 = (size_t)(i - 9);  /* i-8 */
            size_t idx3 = (size_t)(i - 11); /* i-10 */
            cache[i - 1] = cache[idx1] + (cache[idx2] * (seq_t)10)
                                     - (cache[idx3] * (seq_t)10);
        }
    }
    cache_max = n;
    return cache[n - 1];
}
