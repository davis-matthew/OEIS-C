/*
 * A028148  Expansion of 1/((1-4x)(1-7x)(1-8x)(1-12x)).
 * https://oeis.org/A028148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 31*a(n-1) - 344*a(n-2) + 1616*a(n-3) - 2688*a(n-4)
   initial values: a(0)=1, a(1)=31, a(2)=617, a(3)=10079 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;      /* allocated size */
    static idx_t  cache_len = 0;      /* number of initialized terms */

    /* ensure capacity for at least n+1 elements */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n if needed */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[(size_t)i] = (seq_t)1;
        } else if (i == 1) {
            cache[(size_t)i] = (seq_t)31;
        } else if (i == 2) {
            cache[(size_t)i] = (seq_t)617;
        } else if (i == 3) {
            cache[(size_t)i] = (seq_t)10079;
        } else {
            /* compute using unsigned arithmetic in a safe order:
               (T1 + T3) - (T2 + T4) */
            seq_t T1 = cache[(size_t)(i-1)] * 31;
            seq_t T3 = cache[(size_t)(i-3)] * 1616;
            seq_t sum1 = T1 + T3;
            seq_t T2 = cache[(size_t)(i-2)] * 344;
            seq_t T4 = cache[(size_t)(i-4)] * 2688;
            seq_t sum2 = T2 + T4;
            cache[(size_t)i] = sum1 - sum2;
        }
    }
    cache_len = (idx_t)(n + 1);
    return cache[(size_t)n];
}
