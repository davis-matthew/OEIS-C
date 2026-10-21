/*
 * A079333  Duplicate of A063938.
 * https://oeis.org/A079333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 26, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: 7‑smooth numbers (numbers whose prime factors are all ≤ 7) */
/* a(1)=1; a(n)=the n‑th integer of the type 2ᵢ·3ʲ·5ᵏ·7ˡ in increasing order. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of generated numbers */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;
    /* indices for the 4 primes */
    static idx_t i2 = 0, i3 = 0, i5 = 0, i7 = 0;

    /* Grow cache until it contains at least n numbers */
    while (cache_size < n)
    {
        /* initialise the cache on first use */
        if (cache_size == 0)
        {
            cache_cap  = 16;
            cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (cache == NULL) return (seq_t)0;          /* out of memory */
            cache[0]   = (seq_t)1;
            cache_size = 1;
            i2 = i3 = i5 = i7 = 0;
            continue;
        }

        /* compute next candidate values */
        seq_t v2 = cache[i2] * (seq_t)2;
        seq_t v3 = cache[i3] * (seq_t)3;
        seq_t v5 = cache[i5] * (seq_t)5;
        seq_t v7 = cache[i7] * (seq_t)7;

        /* smallest of them is the next number */
        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;
        if (v7 < next) next = v7;

        /* store it */
        if (cache_size >= cache_cap)
        {
            cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            cache      = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (cache == NULL) return (seq_t)0;          /* out of memory */
        }
        cache[cache_size++] = next;

        /* advance all indices that produced the minimum */
        if (v2 == next) ++i2;
        if (v3 == next) ++i3;
        if (v5 == next) ++i5;
        if (v7 == next) ++i7;
    }

    return cache[n - 1];
}
