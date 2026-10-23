/*
 * A017844  Expansion of 1/(1-x^5-x^6-x^7-x^8-x^9-x^10-x^11-x^12-x^13).
 * https://oeis.org/A017844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + a(n-6) + a(n-7) + a(n-8) + a(n-9) + a(n-10) +
 *            a(n-11) + a(n-12) + a(n-13) for n>12, with a(0)=1,
 *            a(1)=a(2)=a(3)=a(4)=0, a(5)=1, a(6)=1, a(7)=1, a(8)=1, a(9)=1,
 *            a(10)=2, a(11)=3, a(12)=4, a(13)=5. The sequence counts
 *            compositions of n into parts 5..13
 * Range:     u8 n <= 27, u16 n <= 48, u32 n <= 89, u64 n <= 171,
 *            u128 n <= 336, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0; /* allocated capacity */
static size_t cache_size = 0; /* next index to compute */

/* ensure that the cache can hold at least 'needed' elements */
static void extend_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* compute up to n if needed */
    if (n < cache_size)
        return cache[(size_t)n];

    extend_cache((size_t)n + 1);

    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i <= 4) {
            cache[i] = (seq_t)0;
        } else if (i == 5) {
            cache[i] = (seq_t)1;
        } else if (i == 6) {
            cache[i] = (seq_t)1;
        } else if (i == 7) {
            cache[i] = (seq_t)1;
        } else if (i == 8) {
            cache[i] = (seq_t)1;
        } else if (i == 9) {
            cache[i] = (seq_t)1;
        } else if (i == 10) {
            cache[i] = (seq_t)2;
        } else if (i == 11) {
            cache[i] = (seq_t)3;
        } else if (i == 12) {
            cache[i] = (seq_t)4;
        } else if (i == 13) {
            cache[i] = (seq_t)5;
        } else {
            seq_t sum = cache[i-5] + cache[i-6] + cache[i-7] + cache[i-8] +
                        cache[i-9] + cache[i-10] + cache[i-11] + cache[i-12] +
                        cache[i-13];
            cache[i] = sum;
        }
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
