/*
 * A231951  Number of (n+1) X (2+1) 0..1 arrays with every element equal to
 *          some horizontal, vertical, diagonal or antidiagonal neighbor, with
 *          top left element zero.
 * https://oeis.org/A231951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 2*a(n-2) + 13*a(n-3) + 8*a(n-4) with a(1)=18,
 *            a(2)=142, a(3)=1034, a(4)=7756. Offset 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;        /* 1‑based cache of computed terms */
static idx_t cache_size = 0;       /* maximum n for which cache contains a(n) */

/* Initial values for n = 1..4 (index 0 unused) */
static const seq_t init[5] = {
    (seq_t)0,
    (seq_t)18,
    (seq_t)142,
    (seq_t)1034,
    (seq_t)7756
};

/* Compute a(n) for any n>=1, memoizing results. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined but harmless */

    /* Allocate initial cache if needed */
    if (cache == NULL) {
        /* allocate space for indices 0..4 */
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;    /* out of memory: fallback */
        for (idx_t i = 1; i <= 4; ++i) cache[i] = init[i];
        cache_size = 4;
    }

    /* Expand cache if n exceeds current capacity */
    if (n > cache_size) {
        /* Allocate new size (n+1) to hold index n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;      /* out of memory: fallback */
        cache = tmp;

        /* Compute new terms from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 4) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(i) = 7*a(i-1) + 2*a(i-2) + 13*a(i-3) + 8*a(i-4) */
                cache[i] = cache[i - 1] * (seq_t)7
                          + cache[i - 2] * (seq_t)2
                          + cache[i - 3] * (seq_t)13
                          + cache[i - 4] * (seq_t)8;
            }
        }
        cache_size = n;
    }

    return cache[n];
}
