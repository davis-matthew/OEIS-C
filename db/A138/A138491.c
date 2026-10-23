/*
 * A138491  Say what you see in previous term, from the right, reporting total
 *          number for each digit encountered. Initial term is 7.
 * https://oeis.org/A138491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   get_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 7
   a(n) = NEXT(a(n-1))
   where NEXT(x) is obtained by scanning x from right to left
   and, for each digit first encountered, appending the total count
   of that digit in x followed by the digit itself. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/get_next.h"

/* Cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

/* Public sequence function */
seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* fallback */
        cache[0] = 7;        /* a(0) */
        cache_len = 1;
    }

    while (cache_len <= n) {
        seq_t next = get_next(cache[cache_len - 1]);
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = next;
        ++cache_len;
    }
    return cache[n];
}
