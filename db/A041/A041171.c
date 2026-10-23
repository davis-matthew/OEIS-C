/*
 * A041171  Denominators of continued fraction convergents to sqrt(95).
 * https://oeis.org/A041171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 78*a(n-4) – a(n-8) Initial values: a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=4, a(4)=75, a(5)=79, a(6)=233, a(7)=312
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, realloc */
#include <stddef.h>    /* size_t */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries already stored */

    /* Return immediately if already computed */
    if (n < cache_len)
        return cache[n];

    /* Desired cache length */
    size_t new_len = (size_t)(n + 1);

    /* Allocate or grow the cache */
    if (cache == NULL) {
        cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failed */
        cache_len = 0;                 /* no entries yet */
    } else if (new_len > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;    /* reallocation failed */
        cache = tmp;
    }

    /* Initial values for indices 0..7 */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)1, (seq_t)3, (seq_t)4,
        (seq_t)75, (seq_t)79, (seq_t)233, (seq_t)312
    };

    /* Fill new entries */
    for (idx_t i = cache_len; i < (idx_t)new_len; ++i) {
        if (i < 8) {
            cache[i] = init[i];
        } else {
            /* Use (seq_t)78 to ensure the multiplication is performed in
               the correct type hierarchy. The subtraction will then use
               the same type. */
            cache[i] = (seq_t)78 * cache[i - 4] - cache[i - 8];
        }
    }

    /* Update cached count */
    cache_len = new_len;

    return cache[n];
}
