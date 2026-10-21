/*
 * A087438  a(n) = 3*2^(2*(n-1)) + 2^(n-2)*(n+1).
 * https://oeis.org/A087438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(2)=15,
   a(n)=8*a(n-1)-20*a(n-2)+16*a(n-3)  (n≥3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t count = 0;

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        capacity = 3;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)15;
        count = 3;
    }

    /* if requested term already cached, return it */
    if (n < (idx_t)count) return cache[(size_t)n];

    /* grow cache if necessary */
    size_t required = (size_t)n + 1;
    if (required > capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* re‑allocation failed */
        cache = tmp;
        capacity = required;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = count; i <= (size_t)n; ++i) {
        seq_t val = 8 * cache[i-1] - 20 * cache[i-2] + 16 * cache[i-3];
        cache[i] = val;
    }
    count = required;

    return cache[(size_t)n];
}
