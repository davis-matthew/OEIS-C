/*
 * A053756  a(n)=a(a(n-3))+a(n-a(n-3)) if n>2 otherwise a(n)=1.
 * https://oeis.org/A053756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A053756
   Definition: a(0)=a(1)=a(2)=1,
               a(n)=a(a(n-3)) + a(n-a(n-3))   for n>2
   Indexing starts at 0. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed terms      */
    static size_t cache_len = 0;          /* number of terms stored       */
    static size_t cache_cap = 0;          /* allocated capacity           */

    /* Expand cache to hold at least n+1 elements. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms up to n if required. */
    if ((size_t)n + 1 > cache_len) {
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            if (i <= 2) {
                cache[i] = (seq_t)1;
            } else {
                idx_t prev = (idx_t)cache[i - 3];          /* a(i-3) fits in idx_t   */
                idx_t j    = prev;                         /* a(i-3) itself          */
                idx_t k    = i - j;                        /* i - a(i-3)             */
                cache[i] = cache[j] + cache[k];
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
