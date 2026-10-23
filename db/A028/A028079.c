/*
 * A028079  Expansion of 1/((1-3x)(1-6x)(1-8x)(1-9x)).
 * https://oeis.org/A028079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028079: expansion of 1/((1-3x)(1-6x)(1-8x)(1-9x))
   Formula: a(n) = (5*9^(n+3) - 9*8^(n+3) + 5*6^(n+3) - 3^(n+3)) / 90
   Implemented by an O(n) recurrence that avoids large intermediate powers. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *a_cache  = NULL;   /* cached a(n) values          */
    static seq_t *f2_cache = NULL;   /* cached f2 values           */
    static seq_t *f3_cache = NULL;   /* cached f3 values           */
    static size_t cache_cap = 0;     /* allocated array size       */
    static idx_t  last_n   = (idx_t)-1; /* highest index cached      */
    static seq_t  f1_pow  = 0;      /* 3^last_n                   */

    /* initialise caches up to index 3 (covers first terms) */
    if (a_cache == NULL) {
        cache_cap = 4;                     /* indices 0..3 will fit      */
        a_cache  = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        f2_cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        f3_cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);

        f1_pow = 1;                         /* 3^0 */
        /* n = 0 */
        f2_cache[0] = f1_pow;
        f3_cache[0] = f2_cache[0];
        a_cache[0]  = f3_cache[0];

        /* n = 1 */
        f1_pow *= 3;                        /* 3^1 */
        f2_cache[1] = f1_pow + 6 * f2_cache[0];
        f3_cache[1] = f2_cache[1] + 8 * f3_cache[0];
        a_cache[1]  = f3_cache[1] + 9 * a_cache[0];

        /* n = 2 */
        f1_pow *= 3;                        /* 3^2 */
        f2_cache[2] = f1_pow + 6 * f2_cache[1];
        f3_cache[2] = f2_cache[2] + 8 * f3_cache[1];
        a_cache[2]  = f3_cache[2] + 9 * a_cache[1];

        /* n = 3 */
        f1_pow *= 3;                        /* 3^3 */
        f2_cache[3] = f1_pow + 6 * f2_cache[2];
        f3_cache[3] = f2_cache[3] + 8 * f3_cache[2];
        a_cache[3]  = f3_cache[3] + 9 * a_cache[2];

        last_n = 3;
    }

    /* extend the cache if the requested index is beyond the current maximum */
    if (n > last_n) {
        /* grow capacity if necessary */
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < needed)
                new_cap <<= 1;  /* double until sufficient */

            a_cache  = (seq_t*)realloc(a_cache,  new_cap * sizeof(seq_t));
            f2_cache = (seq_t*)realloc(f2_cache, new_cap * sizeof(seq_t));
            f3_cache = (seq_t*)realloc(f3_cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* compute values iteratively up to n */
        for (idx_t i = last_n + 1; i <= n; ++i) {
            f1_pow *= 3;                      /* 3^i */
            seq_t f2_i = f1_pow + 6 * f2_cache[i-1];
            f2_cache[i] = f2_i;
            seq_t f3_i = f2_i + 8 * f3_cache[i-1];
            f3_cache[i] = f3_i;
            a_cache[i]  = f3_i + 9 * a_cache[i-1];
        }
        last_n = n;
    }

    return a_cache[n];
}
