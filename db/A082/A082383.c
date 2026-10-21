/*
 * A082383  a(0)=1, a(n)=2^n+n-2*a(n-1).
 * https://oeis.org/A082383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1; a(n)=2^n + n - 2*a(n-1) (n≥2)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* external types supplied by the build */
extern seq_t a(idx_t n);   /* declare to avoid implicit definition */

/* memoisation cache */
static seq_t *cache        = NULL;   /* array of a(i) values */
static size_t cache_len    = 0;      /* number of terms stored (highest index+1) */
static size_t cache_cap    = 0;      /* capacity of the allocated array */

/* current 2^k value used for incremental computation */
static seq_t  pow2_val      = 1;      /* 2^pow2_idx */
static idx_t  pow2_idx      = 0;      /* exponent for pow2_val */

/* enlarge cache to hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;                     /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;

    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr)          /* allocation failed; aborting is acceptable */
        return;

    cache = new_arr;
    cache_cap = new_cap;
}

/* compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* Convert unsigned index to size_t for array access */
    size_t idx = (size_t)n;

    /* Ensure cache covers index 'idx' */
    size_t need = idx + 1;
    if (need > cache_len) {
        /* grow cache to the required size */
        ensure_cache_capacity(need);

        /* compute missing terms sequentially */
        for (size_t i = cache_len; i < need; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                /* Update pow2_val to 2^i */
                while (pow2_idx < i) {
                    pow2_val = pow2_val * (seq_t)2;
                    pow2_idx++;
                }
                /* a(i) = 2^i + i - 2*a(i-1) */
                cache[i] = pow2_val + (seq_t)i - (seq_t)2 * cache[i-1];
            }
        }
        cache_len = need;
    }

    return cache[idx];
}
