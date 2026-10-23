/*
 * A042118  Numerators of continued fraction convergents to sqrt(584).
 * https://oeis.org/A042118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 290*a(n-2) - a(n-4) a(0) = 24, a(1) = 145, a(2) = 6984,
 *            a(3) = 42049
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < cache_size)           /* already computed */
        return cache[n];

    /* grow cache to size n+1 */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)                      /* allocation failed */
        return (seq_t)0;           /* caller must check for failure */
    cache = tmp;
    /* fill new entries */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0)
            cache[i] = 24;
        else if (i == 1)
            cache[i] = 145;
        else if (i == 2)
            cache[i] = 6984;
        else if (i == 3)
            cache[i] = 42049;
        else
            cache[i] = (seq_t)290 * cache[i - 2] - cache[i - 4];
    }
    cache_size = new_size;
    return cache[n];
}
