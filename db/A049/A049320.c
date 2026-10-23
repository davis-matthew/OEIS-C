/*
 * A049320  Non-primitive Chacon sequence: fixed under 0->0010, 1->1.
 * https://oeis.org/A049320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) is the nth term of the fixed point of the morphism 0 →
 *            0010, 1 → 1. The sequence starts 0,0,1,0,0,0,1,0,1,0,0,1
 * Range:     u8 n <= 9810, u16 n <= 9810, u32 n <= 9810, u64 n <= 9810,
 *            u128 n <= 9810, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache holding the sequence up to the current known length */
static seq_t *cache = NULL;   /* array of 0/1 */
static idx_t  cache_len = 0;  /* number of terms stored */

/* generate the sequence until it contains at least n terms */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    /* initialise with the single symbol 0 if necessary */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* expand the cache until we have enough terms */
    while (cache_len < n) {
        /* count zeros and ones in the current sequence */
        idx_t zero_cnt = 0, one_cnt = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i] == 0)
                ++zero_cnt;
            else
                ++one_cnt;
        }

        /* new length after applying the morphism */
        idx_t new_len = zero_cnt * 4 + one_cnt;

        /* allocate buffer for the new sequence */
        seq_t *new_arr = (seq_t*)malloc(sizeof(seq_t) * (size_t)new_len);
        idx_t pos = 0;
        for (idx_t i = 0; i < cache_len; ++i) {
            if (cache[i] == 0) {
                new_arr[pos++] = (seq_t)0;
                new_arr[pos++] = (seq_t)0;
                new_arr[pos++] = (seq_t)1;
                new_arr[pos++] = (seq_t)0;
            } else {
                new_arr[pos++] = (seq_t)1;
            }
        }

        /* replace old cache */
        free(cache);
        cache      = new_arr;
        cache_len  = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, but return 0 */
    ensure_cache(n);
    return cache[n-1];
}
