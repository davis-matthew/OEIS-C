/*
 * A163593  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^5 = I.
 * https://oeis.org/A163593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*a(n-1)+32*a(n-2)+32*a(n-3)+32*a(n-4)-528*a(n-5) with
 *            a(0) = 1, a(1) = 34, a(2) = 1122, a(3) = 37026, a(4) = 1221858,
 *            a(5) = 40320753
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* pointer to stored values */
    static size_t  cap    = 0;      /* allocated capacity (in seq_t elements) */
    static size_t  size   = 0;      /* number of values actually stored */

    /* one‑time initialisation */
    if (cache == NULL) {
        /* start with a modest capacity, will grow as needed */
        cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return (seq_t)0; /* allocation failure – give up */

        /* initialise the first six terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)34;
        cache[2] = (seq_t)1122;
        cache[3] = (seq_t)37026;
        cache[4] = (seq_t)1221858;
        cache[5] = (seq_t)40320753;
        size = 6;
    }

    /* if the requested index is already cached, return it */
    if ((size_t)n < size) return cache[n];

    /* ensure we have enough space for the new terms */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (size_t i = size; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];

        seq_t sum = a1 + a2 + a3 + a4;          /* 32*(a1+a2+a3+a4)  */
        sum = sum * (seq_t)32;
        seq_t sub = a5 * (seq_t)528;
        cache[i] = sum - sub;
    }

    size = (size_t)n + 1;
    return cache[n];
}
