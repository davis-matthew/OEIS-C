/*
 * A095939  a(n+2) = 5a(n+1) - 3a(n) (n >= 1); a(0) = 1, a(1) = 2, a(2) = 9.
 * https://oeis.org/A095939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+2) = 5*a(n+1) - 3*a(n) for n >= 1;
   initial values: a(0)=1, a(1)=2, a(2)=9 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < cache_size)
        return cache[n];

    idx_t old_size = cache_size;
    size_t new_size = (size_t)(n + 1);

    if (cache == NULL) {
        cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
    } else if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;     /* allocation failure */
        cache = tmp;
    }

    for (idx_t i = old_size; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)2;
        else if (i == 2)
            cache[i] = (seq_t)9;
        else
            cache[i] = 5 * cache[i-1] - 3 * cache[i-2];
    }

    cache_size = n + 1;
    return cache[n];
}
