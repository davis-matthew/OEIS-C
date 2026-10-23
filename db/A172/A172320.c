/*
 * A172320  11th column of A172119.
 * https://oeis.org/A172320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+11) = 2*a(n+10) - a(n) Initial values: a(0)=1, a(1)=2,
 *            a(2)=4, a(3)=8, a(4)=16, a(5)=32, a(6)=64, a(7)=128, a(8)=256,
 *            a(9)=512, a(10)=1024, a(11)=2047
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Utility: allocate array of seq_t */
static seq_t *alloc_seq_array(size_t n)
{
    return (seq_t *)malloc(n * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    /* Static cache to allow out‑of‑order calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initialise cache with the first 12 terms */
    if (!cache) {
        cache_size = 12;
        cache = alloc_seq_array(cache_size);
        if (!cache) return 0;         /* allocation failure (unlikely) */

        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)2;
        cache[2]  = (seq_t)4;
        cache[3]  = (seq_t)8;
        cache[4]  = (seq_t)16;
        cache[5]  = (seq_t)32;
        cache[6]  = (seq_t)64;
        cache[7]  = (seq_t)128;
        cache[8]  = (seq_t)256;
        cache[9]  = (seq_t)512;
        cache[10] = (seq_t)1024;
        cache[11] = (seq_t)2047;
    }

    /* If the requested index is already cached, return it */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Grow the cache to hold up to index n */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = alloc_seq_array(new_size);
    if (!new_cache) return 0;        /* allocation failure */

    /* Copy old values into the new cache */
    memcpy(new_cache, cache, cache_size * sizeof(seq_t));
    free(cache);
    cache = new_cache;
    cache_size = new_size;

    /* Compute any missing terms using the recurrence */
    for (idx_t i = 12; i <= n; ++i) {
        /* i-1 and i-11 are guaranteed to be valid */
        cache[i] = 2 * cache[i - 1] - cache[i - 11];
    }

    return cache[n];
}
