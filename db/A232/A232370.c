/*
 * A232370  Number of n X 2 0..3 arrays with every 0 next to a 1, every 1 next
 *          to a 2 and every 2 next to a 3 horizontally, diagonally or
 *          antidiagonally, and no adjacent values equal.
 * https://oeis.org/A232370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A232370
   Empirical recurrence (offset 1):
   a(n) = 2*a(n-1) + 7*a(n-2) + 6*a(n-3)
          - 2*a(n-4) - 3*a(n-5)
          + 2*a(n-6) + a(n-7)
   Initial values:
   a(1)=2, a(2)=14, a(3)=58, a(4)=230, a(5)=934,
   a(6)=3794, a(7)=15354
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* offset is 1 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* 1‑based: cache[0] unused */
    static size_t alloc_size = 0; /* largest index currently allocated */
    static size_t cache_size = 0; /* largest n computed so far */

    /* initialise cache with the first seven values */
    if (cache_size == 0) {
        size_t init = 8;                     /* indices 0..7 */
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) abort();
        alloc_size = init - 1;               /* last valid index after alloc */

        static const seq_t init_vals[7] = {
            (seq_t)2, (seq_t)14, (seq_t)58,
            (seq_t)230, (seq_t)934, (seq_t)3794, (seq_t)15354
        };
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = init_vals[i - 1];
        cache_size = 7;
    }

    /* already computed? */
    if (n <= cache_size)
        return cache[n];

    /* ensure capacity for up to n */
    if ((size_t)n > alloc_size) {
        size_t needed = (size_t)n + 1; /* +1 for index 0 */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        alloc_size = needed - 1; /* last valid index after realloc */
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t part1 = cache[i - 1] * (seq_t)2;
        seq_t part2 = cache[i - 2] * (seq_t)7;
        seq_t part3 = cache[i - 3] * (seq_t)6;
        seq_t part4 = cache[i - 4] * (seq_t)2;
        seq_t part5 = cache[i - 5] * (seq_t)3;
        seq_t part6 = cache[i - 6] * (seq_t)2;
        seq_t part7 = cache[i - 7];

        seq_t pos = part1 + part2 + part3 + part6 + part7;
        seq_t neg = part4 + part5;

        cache[i] = pos - neg;
    }
    cache_size = n;
    return cache[n];
}
