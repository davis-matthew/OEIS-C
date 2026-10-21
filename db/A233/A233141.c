/*
 * A233141  Number of n X 2 0..5 arrays with no element x(i,j) adjacent to
 *          value 5-x(i,j) horizontally, antidiagonally or vertically, top
 *          left element zero, and 1 appearing before 2 3 and 4, and 2
 *          appearing before 3 in row major order.
 * https://oeis.org/A233141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1) - 134*a(n-2) + 134*a(n-3) - 25*a(n-4) + a(n-5)
 *            initial values: a(1)=2, a(2)=16, a(3)=225, a(4)=3727, a(5)=65120
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External function required by the build */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;
    /* first five terms (index 0 unused) */
    static const seq_t init[6] = {0, 2, 16, 225, 3727, 65120};

    if (n <= 5) return init[n];

    /* initialise cache with the first five terms if not already done */
    if (cached_len < 5) {
        size_t size = 6;                /* indices 0..5 */
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed – undefined behaviour */
        for (idx_t i = 1; i <= 5; ++i) cache[i] = init[i];
        cached_len = 5;
    }

    /* grow cache if necessary */
    if (n > cached_len) {
        size_t new_size = (size_t)n + 1;    /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed – undefined behaviour */
        cache = tmp;
        for (idx_t i = cached_len + 1; i <= n; ++i) {
            seq_t a1 = cache[i-1];   /* a(n-1) */
            seq_t a2 = cache[i-2];   /* a(n-2) */
            seq_t a3 = cache[i-3];   /* a(n-3) */
            seq_t a4 = cache[i-4];   /* a(n-4) */
            seq_t a5 = cache[i-5];   /* a(n-5) */
            seq_t val = 25*a1 + 134*a3 + a5 - 134*a2 - 25*a4;
            cache[i] = val;
        }
        cached_len = n;
    }

    return cache[n];
}
