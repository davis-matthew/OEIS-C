/*
 * A237632  Number of (n+1) X (3+1) 0..2 arrays with the upper median of every
 *          2 X 2 subblock differing from its horizontal and vertical
 *          neighbors by exactly one.
 * https://oeis.org/A237632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-4) for n > 13 (empirical recurrence); initial
 *            values: a(1)=702, a(2)=1220, a(3)=1310, a(4)=1488, a(5)=2086,
 *            a(6)=2900, a(7)=4132, a(8)=5720, a(9)=8248, a(10)=11504,
 *            a(11)=16546, a(12)=22928, a(13)=33024
 * Range:     u8 none, u16 n <= 14, u32 n <= 46, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values (1‑based indexing; cache[0] unused) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* first 13 terms (index 1‑based) */
    static const seq_t init[14] = {0,
        702,1220,1310,1488,2086,2900,4132,5720,8248,11504,16546,
        22928,33024};

    if (n < 1) return 0;                       /* undefined for n <= 0 */
    if (n <= 13) return init[(size_t)n];      /* directly return initial value */

    /* allocate or enlarge cache if necessary */
    if (cache == NULL) {
        cache_size = (size_t)n + 1;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failed */
        for (size_t i = 1; i <= 13; ++i) cache[i] = init[i];
    } else if (cache_size <= (size_t)n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute any missing entries up to n using the recurrence */
    for (size_t i = 14; i <= (size_t)n; ++i)
        cache[i] = cache[i-4] * (seq_t)4;

    return cache[(size_t)n];
}
