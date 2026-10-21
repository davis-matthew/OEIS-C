/*
 * A088462  a(1)=1, a(n) = ceiling((n - a(a(n-1)))/2).
 * https://oeis.org/A088462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - a(n-1) - a(a(n-2)), a(1)=a(2)=1
 * Range:     u8 n <= 616, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* static cache for memoisation */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;

/* Ensure the cache can hold values up to index n */
static void ensure_a_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;           /* index 0 unused */
    if (needed <= a_cache_cap)
        return;

    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    while (new_cap < needed) {
        if (new_cap > SIZE_MAX / 2) {
            new_cap = needed;
            break;
        }
        new_cap *= 2;
    }

    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                      /* out of memory */

    /* initialise new memory to zero */
    memset(tmp + a_cache_cap, 0, (new_cap - a_cache_cap) * sizeof(seq_t));

    a_cache = tmp;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return (seq_t)1;

    ensure_a_cache(n);
    if (a_cache[n] != (seq_t)0)
        return a_cache[n];

    /* compute using the recurrence */
    seq_t prev = a(n - 1);
    seq_t inner_idx = a(n - 2);           /* a(n-2) is small enough for idx_t */
    seq_t inner = a((idx_t)inner_idx);

    seq_t result = (seq_t)n - prev - inner;
    a_cache[n] = result;
    return result;
}
