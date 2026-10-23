/*
 * A011678  A binary m-sequence: expansion of reciprocal of x^7+x^5+x^3+x+1.
 * https://oeis.org/A011678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) XOR a(n-3) XOR a(n-5) XOR a(n-7), with
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=1. The sequence
 *            is 127‑periodic
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache storing computed values */
    static seq_t *cache = NULL;
    static size_t  cache_size   = 0;   /* allocated array size     */
    static idx_t   computed_upto = 0;   /* highest index already computed */

    /* first call: initialise 0..6 */
    if (cache == NULL) {
        cache_size = 8;                            /* 0..7 at least */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) { exit(1); }

        /* explicit initial values */
        cache[0] = 0; cache[1] = 0; cache[2] = 0;
        cache[3] = 0; cache[4] = 0; cache[5] = 0;
        cache[6] = 1;

        /* we treat cache[7] as uninitialised until needed */
        computed_upto = 6;
    }

    /* if the requested term is already known, return it */
    if (n <= computed_upto)
        return cache[n];

    /* enlarge the cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = cache_size;
        if (new_size == 0) new_size = 8;
        while ((size_t)n + 1 > new_size)
            new_size *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) { exit(1); }
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing values up to n */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        /* recurrence indices are guaranteed < i, so already computed */
        seq_t val = cache[i-1] ^ cache[i-3] ^ cache[i-5] ^ cache[i-7];
        cache[i] = val;
    }
    computed_upto = n;

    return cache[n];
}
