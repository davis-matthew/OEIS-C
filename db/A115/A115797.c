/*
 * A115797  Integers i such that 3*i XOR 8*i = 11*i.
 * https://oeis.org/A115797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Integers i such that (3*i) XOR (8*i) == 11*i. The function
 *            returns the n‑th term (n ≥ 1) of OEIS sequence A115797
 * Range:     u8 n <= 27, u16 n <= 1006, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache to store found terms and to keep the search state between calls. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* capacity of the cache array      */
    static idx_t tested_i = 0;     /* next integer to be tested */

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < n)
    {
        seq_t i_seq = (seq_t)tested_i;           /* current candidate */
        seq_t three = i_seq * 3;                 /* 3*i          */
        seq_t eight = i_seq << 3;                /* 8*i          */

        /* Condition: (3*i) & (8*i) == 0  <=>  (3*i) XOR (8*i) == 11*i  */
        if ((three & eight) == 0)
        {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) return 0; /* allocation failed – return 0 as sentinel */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = i_seq;  /* store the term */
        }
        tested_i++; /* move to next integer */
    }

    return cache[n - 1]; /* 1‑based index: a(1) is the first term */
}
