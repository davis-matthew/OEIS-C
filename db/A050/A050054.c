/*
 * A050054  a(n) = a(n-1) + a(m) for n >= 4, where m = n - 1 - 2^p and p is
 *          the unique integer such that 2^p < n - 1 <= 2^(p+1), starting with
 *          a(1) = 1, a(2) = 2, and a(3) = 4.
 * https://oeis.org/A050054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 191, u32 n <= 1674, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1, a(2) = 2, a(3) = 4;
   For n ≥ 4,
   let t = n - 1,
   p  = floor_log2(t - 1)          // largest integer such that 2^p < t ≤ 2^(p+1)
   m  = t - 2^p
   then a(n) = a(n-1) + a(m) */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
 
/* floor_log2 for unsigned 64-bit values */
static inline idx_t floor_log2_u64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}
 
/* Cached sequence values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
 
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
 
    if (n >= cache_size) {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!new_cache) abort();           /* allocation failed */
        cache = new_cache;
        cache_size = new_size;
        for (idx_t k = old_size; k <= n; ++k) {
            if (k == 0) {
                cache[0] = (seq_t)0;
            } else if (k == 1) {
                cache[1] = (seq_t)1;
            } else if (k == 2) {
                cache[2] = (seq_t)2;
            } else if (k == 3) {
                cache[3] = (seq_t)4;
            } else {
                idx_t t  = k - 1;
                idx_t p  = floor_log2_u64(t - 1);
                idx_t pow2 = ((idx_t)1) << p;
                idx_t m = t - pow2;
                cache[k] = cache[k - 1] + cache[m];
            }
        }
    }
    return cache[n];
}
