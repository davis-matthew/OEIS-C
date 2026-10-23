/*
 * A016234  Expansion of 1/((1-x) * (1-5*x) * (1-9*x)).
 * https://oeis.org/A016234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A016234: Expansion of 1/((1-x)*(1-5x)*(1-9x)).
 * Recurrence:
 *   a(0) = 1,
 *   a(1) = 15,
 *   a(n) = 14*a(n-1) - 45*a(n-2) + 1   (n >= 2)
 *
 * To avoid overflow on fixed-size integer builds the multiplication
 * by the small constants 14 and 45 is performed by repeated addition,
 * which is exact for all n where the final result fits into seq_t.
 * A simple static cache allows O(n) computation even when calls come
 * in arbitrary order.
 */
#include <stdlib.h>

/* multiply seq_t by a small unsigned integer using repeated addition */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    seq_t r = 0;
    for (unsigned int i = 0; i < m; ++i)
        r += x;
    return r;
}

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;
    static idx_t  last  = 0;

    /* ensure cache is initialized for at least first two terms */
    if (cap == 0) {
        cap = 4;                    /* start with capacity 4 */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure, return 0 */
        cache[0] = 1;
        cache[1] = 15;
        last = 1;
    }

    /* handle initial terms */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* expand cache if needed */
    if ((idx_t)n > last) {
        /* grow capacity as needed */
        idx_t new_cap = cap;
        while ((idx_t)new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing values */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = mul_small(cache[i-1], 14);
        term -= mul_small(cache[i-2], 45);
        term += 1;
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
