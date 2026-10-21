/*
 * A213769  Principal diagonal of the convolution array A213768.
 * https://oeis.org/A213769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - 2*a(n-3) + 4*a(n-4) - a(n-6) with
 *            a(1)=1,a(2)=8,a(3)=26,a(4)=63,a(5)=136,a(6)=272
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 36, u64 n <= 81, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] unused, cache[i] = a(i) */
static size_t cache_len = 0;  /* currently allocated size (max index + 1) */

static void ensure_cache(idx_t n)
{
    /* Allocate or grow cache to hold indices up to n */
    if (n >= cache_len) {
        size_t new_len = n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!cache) abort();
            cache_len = new_len;
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_len = new_len;
        }
        /* Initialize known values a(1)..a(6) */
        if (cache_len > 1) cache[1] = (seq_t)1;
        if (cache_len > 2) cache[2] = (seq_t)8;
        if (cache_len > 3) cache[3] = (seq_t)26;
        if (cache_len > 4) cache[4] = (seq_t)63;
        if (cache_len > 5) cache[5] = (seq_t)136;
        if (cache_len > 6) cache[6] = (seq_t)272;
        /* Compute remaining terms using the recurrence */
        for (idx_t i = 7; i <= n; ++i) {
            seq_t t1 = 4 * cache[i-1] + 4 * cache[i-4];
            seq_t t2 = 4 * cache[i-2] + 2 * cache[i-3] + cache[i-6];
            cache[i] = t1 - t2;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; undefined for n==0 */
    if (n <= 6) {
        static const seq_t base[7] = {0, 1, 8, 26, 63, 136, 272};
        return base[n];
    }
    ensure_cache(n);
    return cache[n];
}
