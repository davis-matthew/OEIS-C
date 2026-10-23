/*
 * A197192  a(n) = binomial(n+9, 9)*7^n.
 * https://oeis.org/A197192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+9,9)*7^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the OEIS sequence A197192:
   a(n) = C(n+9,9) * 7^n
   with memoization to handle repeated calls efficiently. */
seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values already stored */

    /* allocate initial cache space if needed */
    if (!cache) {
        cache_len = 1;                          /* store a(0) */
        cache = (seq_t *)malloc(sizeof(seq_t) * 64); /* initial capacity */
        cache[0] = (seq_t)1;                   /* a(0) = 1 */
    }

    /* grow cache if we need to compute new terms up to index n */
    if (n >= cache_len) {
        /* determine new capacity */
        size_t new_cap = cache_len ? cache_len * 2 : 64;
        while (new_cap <= n)
            new_cap <<= 1;                       /* exponential growth */

        /* reallocate cache */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            /* allocation failed – abort (unlikely in the test environment) */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                seq_t prev = cache[i - 1];
                seq_t num  = (seq_t)(i) + (seq_t)9; /* (i+9)   */
                seq_t den  = (seq_t) i;             /* i       */
                seq_t prod = (prev * num) / den;    /* C(i+9,9) factor */
                cache[i] = prod * (seq_t)7;         /* 7^i factor */
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
