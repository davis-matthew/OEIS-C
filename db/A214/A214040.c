/*
 * A214040  a(n)=a(n-1)+floor((a(n-2)+a(n-3))/2), with a(n)=n for n<3.
 * https://oeis.org/A214040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + floor((a(n-2)+a(n-3))/2), a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the nth term of OEIS sequence A214040. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached values */
    static size_t sz   = 0;       /* number of terms stored      */
    static size_t cap  = 0;       /* capacity of the cache array */
    /* initialise cache once */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory – unlikely for n<10000 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        sz = 3;
    }
    /* If the requested index is already cached, return it. */
    if ((size_t)n < sz) return cache[(size_t)n];

    /* Ensure the cache can hold up to index n. */
    size_t new_sz = (size_t)n + 1;
    if (new_sz > cap) {
        size_t new_cap = cap;
        while (new_cap < new_sz) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out of memory – unlikely for n<10000 */
        cache = tmp;
        cap = new_cap;
    }
    /* Compute missing terms sequentially. */
    for (size_t i = sz; i < new_sz; ++i) {
        seq_t s = cache[i - 2] + cache[i - 3];
        seq_t half = s / (seq_t)2;               /* floor((a(i-2)+a(i-3))/2) */
        cache[i] = cache[i - 1] + half;
    }
    sz = new_sz;
    return cache[(size_t)n];
}
