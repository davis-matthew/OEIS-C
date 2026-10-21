/*
 * A217757  Product_{i=0..n} (i! + 1).
 * https://oeis.org/A217757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=0..n} (i! + 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage required by the harness */
seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache      = NULL;   /* array for a[0..cur_n]          */
    static size_t  capacity  = 0;      /* current allocated capacity     */
    static idx_t   cur_n     = (idx_t)-1; /* highest computed index */
    static seq_t   fact      = 0;      /* running factorial (i!)         */

    /* Initialisation: compute a(0) and 0! */
    if (cache == NULL) {
        capacity = 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)2;         /* 0! + 1 */
        fact    = (seq_t)1;          /* 0!   */
        cur_n   = 0;
    }

    /* Return cached value if already available */
    if (n <= cur_n)
        return cache[n];

    /* Ensure enough capacity for indices up to n */
    if ((size_t)(n + 1) > capacity) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = cur_n + 1; i <= n; ++i) {
        fact = fact * (seq_t)i;                 /* i!  */
        cache[i] = cache[cur_n] * (fact + (seq_t)1); /* a(i) = a(i-1)*(i!+1) */
        cur_n = i;
    }

    return cache[n];
}
