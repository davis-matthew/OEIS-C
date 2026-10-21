/*
 * A099464  Trisection of tribonacci numbers.
 * https://oeis.org/A099464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7*a(n-1) - 5*a(n-2) + a(n-3) initial values: a(0)=0,
 *            a(1)=1, a(2)=7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes A099464 (trisection of tribonacci numbers). */
seq_t a(idx_t n)
{
    /* Cached values from previous calls. */
    static seq_t *cache = NULL;      /* consecutive terms a(0)...a(cache_len-1) */
    static size_t cache_len = 0;     /* number of stored terms */
    static size_t cache_cap = 0;     /* allocated capacity of cache */

    /* Ensure storage for terms up to index n. */
    size_t required = (size_t)n + 1;
    if (cache_len == 0) {
        /* On first call allocate a small buffer to hold at least 3
           terms (initial values). */
        size_t new_cap = 8;
        while (new_cap < required || new_cap < 3)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    } else if (required > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < required)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first three values if this is the first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache_len = 3;
    }

    /* Compute any missing terms up to n. */
    if ((size_t)n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            cache[i] = ((seq_t)7) * a1 - ((seq_t)5) * a2 + a3;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
