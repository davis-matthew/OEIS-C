/*
 * A049927  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 3.
 * https://oeis.org/A049927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049927
   Recurrence:
      a(1) = 1,  a(2) = a(3) = 3
      for n ≥ 4
      a(n) = (a(1)+a(2)+…+a(n-1)) – a(m)
      where  m = 2*n – 2 – 2^(p+1)
           p = unique integer such that 2^p < n-1 ≤ 2^(p+1)   */

#include <stdint.h>
#include <stdlib.h>

/* compute floor(log2(v)) for 0 < v < 2^64 */
static inline idx_t floor_log2_uint64(idx_t v)
{
    return 63 - __builtin_clzll((unsigned long long)v);
}

/* cached values of a(n) */
static seq_t *cache       = NULL;   /* array of length cached_n */
static idx_t   cached_n   = 0;       /* number of terms already computed */
static seq_t   sum_cache  = 0;       /* sum of cache[0..cached_n-1] */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined, guard */

    /* Allocate or grow cache to hold up to n terms */
    if (n > cached_n) {
        size_t new_size = (size_t)n;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        /* Compute missing terms */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)1;
                sum_cache = cache[0];
            } else if (i == 2 || i == 3) {
                cache[i - 1] = (seq_t)3;
                sum_cache += cache[i - 1];
            } else {
                /* Determine p and m */
                idx_t x = i - 1;                    /* n-1 */
                idx_t p = floor_log2_uint64(x - 1);  /* 2^p < x ≤ 2^(p+1) */
                idx_t power = ((idx_t)1 << (p + 1));
                idx_t m = 2 * i - 2 - power;        /* 1 ≤ m < i */

                seq_t term = sum_cache - cache[m - 1];
                cache[i - 1] = term;
                sum_cache += term;
            }
        }
        cached_n = n;
    }

    return cache[n - 1];
}
