/*
 * A097773  Pell equation solutions (13*b(n))^2 - 170*a(n)^2 = -1 with b(n) =
 *          A097772(n), n >= 0.
 * https://oeis.org/A097773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 678*a(n-1) - a(n-2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 677                       */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size  = 0;

    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)677;

    if (cache == NULL) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;              /* graceful failure if allocation fails */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)677;
        size = 2;
    }

    if (size <= n) {
        /* grow cache to hold indices up to n */
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;               /* graceful failure */
        cache = tmp;
        for (idx_t i = size; i <= n; ++i) {
            cache[i] = (seq_t)678 * cache[i-1] - cache[i-2];
        }
        size = n + 1;
    }

    return cache[n];
}
