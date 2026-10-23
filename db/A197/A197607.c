/*
 * A197607  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 4,2,1,1,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 4*a(n-3) + a(n-4) + 2*a(n-5) +
 *            2*a(n-6) - a(n-7) - a(n-8) Initial values: a(1)=1, a(2)=5,
 *            a(3)=14, a(4)=43, a(5)=124, a(6)=352, a(7)=1025, a(8)=2977
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static memoization storage */
    static seq_t *cache = NULL;
    static idx_t  cached_len = 0;   /* number of terms currently stored (>=8 after init) */
    static idx_t  cached_cap = 0;   /* allocated capacity (>=cached_len) */

    /* initial terms (1-indexed, stored 0..7) */
    static const seq_t init_vals[8] = {1, 5, 14, 43, 124, 352, 1025, 2977};

    /* initialize cache once */
    if (cache == NULL) {
        /* allocate capacity at least 8 */
        cached_cap = 8;
        cached_len = 8;
        cache = (seq_t *)malloc(cached_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
    }

    /* enlarge cache if needed */
    if (n > cached_cap) {
        size_t new_cap = cached_cap ? cached_cap : 8;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cached_cap = (idx_t)new_cap;
    }

    /* compute missing terms up to n */
    if (n > cached_len) {
        for (idx_t k = cached_len + 1; k <= n; ++k) {
            /* k >= 9 ensures indices below are valid */
            seq_t term = 2 * cache[k - 2];
            term += cache[k - 3];
            term += 4 * cache[k - 4];
            term += cache[k - 5];
            term += 2 * cache[k - 6];
            term += 2 * cache[k - 7];
            term -= cache[k - 8];
            term -= cache[k - 9];
            cache[k - 1] = term;
        }
        cached_len = n;
    }

    return cache[n - 1];
}
