/*
 * A206419  Fibonacci sequence beginning 11, 7.
 * https://oeis.org/A206419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 11, a(2) = 7; a(n) = a(n-1) + a(n-2) for n > 2
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding the sequence from a(1) onward */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of elements currently allocated */
    static idx_t  cache_len = 0;   /* number of elements actually filled */

    /* the sequence is only defined for n >= 1 */
    if (n < 1) return (seq_t)0;

    /* grow the cache if required; use realloc to keep the allocation
       minimal while still being safe for arbitrary‑precision types */
    if ((size_t)n > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache     = tmp;
        cache_cap = (size_t)n;
    }

    /* initialise the first element if it is not yet present */
    if (cache_len < 1) {
        cache[0] = (seq_t)11;   /* a(1) */
        cache_len = 1;
    }

    /* initialise the second element if it is required and we have space */
    if (cache_len < 2 && cache_cap >= 2) {
        cache[1] = (seq_t)7;    /* a(2) */
        cache_len = 2;
    }

    /* compute any missing terms up to index n using the recurrence */
    while (cache_len < n) {
        cache[cache_len] = cache[cache_len - 1] + cache[cache_len - 2];
        ++cache_len;
    }

    return cache[n - 1];
}
