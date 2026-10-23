/*
 * A110526  a(n+3) = 3*a(n+2) + 5*a(n+1) + a(n), a(0) = 0, a(1) = 1, a(2) = 3.
 * https://oeis.org/A110526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+3) = 3*a(n+2) + 5*a(n+1) + a(n), a(0)=0, a(1)=1, a(2)=3 */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        cache_cap = 4;          /* initial capacity */
        cache_size = 3;         /* known terms 0..2 */
        cache = seq_malloc(cache_cap);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
    }

    if (n < cache_size)
        return cache[n];

    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap <<= 1;
        cache = seq_realloc(cache, new_cap);
        cache_cap = new_cap;
    }

    idx_t i = cache_size;
    if (i < 3) i = 3;   /* first index that satisfies the recurrence */
    for (; i <= n; ++i) {
        cache[i] = 3 * cache[i-1] + 5 * cache[i-2] + cache[i-3];
    }
    cache_size = n + 1;
    return cache[n];
}
