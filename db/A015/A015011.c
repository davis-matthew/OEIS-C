/*
 * A015011  q-factorial numbers for q=11.
 * https://oeis.org/A015011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=1..n} (11^k - 1) / (11 - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values and auxiliary data */
    static seq_t *cache = NULL;          /* array of terms a(0) .. a(last_n) */
    static idx_t  last_n = 0;            /* highest index currently stored */
    static seq_t   pow11 = (seq_t)1;     /* 11^pow_i */
    static idx_t   pow_i = 0;            /* current exponent of pow11 */

    /* initialize cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t)); /* size 1 for a(0) */
        if (cache == NULL) return (seq_t)0;     /* out of memory --> return 0 */
        cache[0] = (seq_t)1;                    /* a(0) = 1 */
        last_n = 0;
        pow11 = (seq_t)1;                       /* 11^0 */
        pow_i = 0;
    }

    /* if requested n is beyond current cache, extend and compute */
    if (n > last_n) {
        /* allocate bigger array */
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) { /* allocation failed */
            return (seq_t)0;
        }
        cache = new_cache;

        /* compute missing terms */
        for (idx_t i = last_n + 1; i <= n; ++i) {
            /* update pow11 to 11^i */
            pow11 = pow11 * (seq_t)11;
            ++pow_i;

            /* a(i) = (pow11 - 1) * a(i-1) / 10 */
            seq_t factor = pow11 - (seq_t)1;   /* divisible by 10 by construction */
            factor = factor / (seq_t)10;      /* exact division */
            cache[i] = factor * cache[i - 1];
        }
        last_n = n;
    }

    return cache[n];
}
