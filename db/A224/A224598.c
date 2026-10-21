/*
 * A224598  Number of (n+2)X(n+2) 0..2 matrices with each 3X3 subblock
 *          idempotent.
 * https://oeis.org/A224598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A224598 a(n) = 6*a(n-1) -15*a(n-2) +23*a(n-3) -30*a(n-4)
 *            +36*a(n-5) -34*a(n-6) +27*a(n-7) -21*a(n-8) +13*a(n-9) -
 *            6*a(n-10) + 3*a(n-11) - a(n-12) for n > 14 Initial values: (n =
 *            1..14) 122, 272, 482, 666, 1202, 2092, 3308, 5398, 8876, 14232,
 *            22734, 36346, 57712, 91204
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

static const seq_t init_vals[15] = {
    [0] = 0,
    [1] = 122,  [2] = 272,  [3] = 482,  [4] = 666,
    [5] = 1202, [6] = 2092, [7] = 3308, [8] = 5398,
    [9] = 8876, [10] = 14232, [11] = 22734, [12] = 36346,
    [13] = 57712, [14] = 91204
};

/* Compute a(n) for n >= 1.  Deferred allocation and memoization
   guarantee O(n) time for successive calls, even if called in arbitrary
   order. */
seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;    /* 1-based array of computed values */
    static size_t cap = 0;         /* allocated number of slots (>= n+1) */
    static idx_t max_computed = 0; /* largest index that has been computed */

    if (n == 0)        /* offset 1 is defined; return 0 for n=0 */
        return 0;

    /* first call: allocate space for 0..14 */
    if (!cache) {
        cap = 15;   /* indices 0..14 inclusive */
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 1; i <= 14; ++i)
            cache[i] = init_vals[i];
        max_computed = 14;
    }

    /* extend cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = ((size_t)n + 1) * 2;   /* growth factor > 1 */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* if already computed, return */
    if (n <= max_computed)
        return cache[n];

    /* compute new terms from max_computed+1 up to n */
    idx_t start = max_computed + 1;
    for (idx_t i = start; i <= n; ++i) {
        cache[i] =  6 * cache[i-1]
                  -15 * cache[i-2]
                  +23 * cache[i-3]
                  -30 * cache[i-4]
                  +36 * cache[i-5]
                  -34 * cache[i-6]
                  +27 * cache[i-7]
                  -21 * cache[i-8]
                  +13 * cache[i-9]
                  - 6 * cache[i-10]
                  + 3 * cache[i-11]
                  -   cache[i-12];
    }
    max_computed = n;
    return cache[n];
}
