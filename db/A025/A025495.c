/*
 * A025495  Probable extension of A013704.
 * https://oeis.org/A025495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 34, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025495
 *  a(0) = 1, a(1) = 2, a(2) = 12, a(3) = 44, a(4) = 100,
 *  a(5) = 203, a(6) = 344, a(7) = 558, a(8) = 824
 *  For n >= 9:
 *  a(n) = 3*a(n-1)-a(n-2)-5*a(n-3)+5*a(n-4)+a(n-5)-3*a(n-6)+a(n-7)
 *  (Linear recurrence of order 7 with constant integer coefficients)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms for n = 0 … 8 */
    static const seq_t init[9] = {
        (seq_t)1, (seq_t)2, (seq_t)12, (seq_t)44, (seq_t)100,
        (seq_t)203, (seq_t)344, (seq_t)558, (seq_t)824
    };

    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size of cache */
    static size_t last_computed = 0; /* highest index already computed */

    /* Return stored initial value if requested */
    if (n <= (idx_t)8)
        return init[(size_t)n];

    /* Allocate cache if not yet done */
    if (!cache) {
        capacity = (size_t)n + 1;
        if (capacity < 9) capacity = 9;          /* enough for init terms */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failed */
        for (size_t i = 0; i < 9; ++i)           /* copy initial terms */
            cache[i] = init[i];
        last_computed = 8;
    }
    /* Grow cache if needed */
    else if (capacity <= (size_t)n) {
        capacity = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failed */
        cache = tmp;
    }

    /* Compute missing values up to n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        /* Compute positive and negative parts separately to avoid
           underflow on unsigned types. */
        seq_t pos = 3 * cache[i - 1] + 5 * cache[i - 4] + cache[i - 5]
                + cache[i - 7];
        seq_t neg = cache[i - 2] + 5 * cache[i - 3] + 3 * cache[i - 6];
        cache[i] = pos - neg;
    }

    last_computed = (size_t)n;
    return cache[(size_t)n];
}
