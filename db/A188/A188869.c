/*
 * A188869  Number of n X 4 binary arrays without the pattern 0 0 0
 *          antidiagonally or horizontally.
 * https://oeis.org/A188869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 13*a(n-1) -16*a(n-2) -8*a(n-3) -44*a(n-4) +109*a(n-5) -53*a(n-6) +4*a(n-7) for n>8;
   Initial values: a(1)=13, a(2)=169, a(3)=1901, a(4)=21937, a(5)=252932,
   a(6)=2915832, a(7)=33617513, a(8)=387583973 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[i] holds a(i+1) for i >= 0 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of elements stored */

    if (n == 0) return (seq_t)0;      /* undefined for the offset 1 */

    /* ensure cache is ready with the 8 initial terms */
    if (cache_len == 0) {
        const seq_t init[8] = {
            (seq_t)13ULL, (seq_t)169ULL, (seq_t)1901ULL,
            (seq_t)21937ULL, (seq_t)252932ULL, (seq_t)2915832ULL,
            (seq_t)33617513ULL, (seq_t)387583973ULL
        };
        cache_len = 8;
        cache = (seq_t *)realloc(NULL, cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        for (size_t i = 0; i < cache_len; ++i) cache[i] = init[i];
    }

    /* return directly if requested term is already cached */
    if ((size_t)n <= cache_len) return cache[n-1];

    /* grow the cache to accommodate new terms, computing them */
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;   /* allocation failed */
    cache = new_cache;

    for (size_t i = cache_len; i < new_len; ++i) {
        /* i corresponds to a(i+1).   Need a(i), a(i-1), …, a(i-6) */
        seq_t p = cache[i-1];            /* a(n-1) */
        seq_t n2 = cache[i-2];           /* a(n-2) */
        seq_t n3 = cache[i-3];           /* a(n-3) */
        seq_t n4 = cache[i-4];           /* a(n-4) */
        seq_t n5 = cache[i-5];           /* a(n-5) */
        seq_t n6 = cache[i-6];           /* a(n-6) */
        seq_t n7 = cache[i-7];           /* a(n-7) */

        seq_t pos = p * (seq_t)13 + n5 * (seq_t)109 + n7 * (seq_t)4;
        seq_t neg = n2 * (seq_t)16 + n3 * (seq_t)8 + n4 * (seq_t)44 + n6 * (seq_t)53;
        cache[i] = pos - neg;
    }
    cache_len = new_len;
    return cache[n-1];
}
