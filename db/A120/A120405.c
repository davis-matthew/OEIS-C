/*
 * A120405  a(n) = 1, a(2) = 1, then append the dot product of (1,2) and (1,1)
 *          = 1*1, 1*2 = 1, 2; to the right of 1, 1; getting (1, 1, 1, 2). The
 *          next operation uses the dot product of (1, 2, 3, 4) and (1, 1, 1,
 *          2), getting (1, 2, 3, 8) which we append to the right of (1, 1, 1,
 *          2), getting (1, 1, 1, 2, 1, 2,...
 * https://oeis.org/A120405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For n > 1 let L = 2^⌊log₂(n-1)⌋ (largest power of two
 *            less than n), m = n - L. Then a(n) = m * a(m). This recurrence
 *            produces the sequence A120405
 * Range:     u8 n <= 30, u16 n <= 119, u32 n <= 507, u64 n <= 4094,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* array of precomputed a(n) */
static idx_t cache_size = 0;     /* currently stored values are cache[1..cache_size] */

/* Compute a(n) with memoization */
seq_t a(idx_t n) {
    /* Ensure cache covers up to index n */
    if (n > cache_size) {
        idx_t new_size = n + 1;                      /* include index 0 unused */
        size_t new_bytes = (size_t)new_size * sizeof(seq_t);
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_bytes);
        } else {
            cache = (seq_t *)realloc(cache, new_bytes);
        }
        /* Initialize from previous size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)1;
            } else {
                idx_t t = i - 1;                     /* i-1 > 0 when i>1 */
                /* compute floor(log2(t)) using builtin */
                unsigned long long tt = (unsigned long long)t;
                idx_t hb = 63 - __builtin_clzll(tt);
                idx_t l = (idx_t)1 << hb;            /* largest power of two < i */
                idx_t m = i - l;
                cache[i] = (seq_t)m * cache[m];
            }
        }
        cache_size = n;
    }
    return cache[n];
}
