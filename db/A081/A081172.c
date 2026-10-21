/*
 * A081172  Tribonacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3), with a(0) =
 *          1, a(1) = 1, a(2) = 0.
 * https://oeis.org/A081172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3), a(0)=1, a(1)=1, a(2)=0
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 75, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: grows as needed, holds computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of allocated slots */

    /* Allocate initial cache if first call */
    if (cache_size == 0) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);          /* out‑of‑memory */
        cache[0] = (seq_t)1;                     /* a(0) */
        cache[1] = (seq_t)1;                     /* a(1) */
        cache[2] = (seq_t)0;                     /* a(2) */
    }
    /* Grow cache if requested n is beyond current allocation */
    if (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        while (new_size <= n) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);            /* out‑of‑memory */
        cache = tmp;
        cache_size = new_size;
    }
    /* Compute missing terms up to n (sequence is forward‑recurrence) */
    for (idx_t i = (cache_size > 3 ? 3 : 3); i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3];
    }
    return cache[n];
}
