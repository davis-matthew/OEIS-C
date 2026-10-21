/*
 * A052926  Expansion of (1-3*x)/(1 - 4*x - x^2 + 3*x^3).
 * https://oeis.org/A052926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 3*a(n-3); a(0)=1, a(1)=1, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Simple memoised DP with a dynamic array */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* current capacity of the array   */
    static idx_t last  = 0;   /* highest index that has been filled */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap   = 4;                     /* at least enough for a(0..3)   */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        last = 2;
    }

    /* For n beyond the cache, grow it */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;          /* double until enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Return early for the first few values */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];
    if (n == 2) return cache[2];

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = (seq_t)4 * cache[i - 1] + cache[i - 2] - (seq_t)3 * cache[i - 3];
        cache[i] = val;
    }
    last = n;

    return cache[n];
}
