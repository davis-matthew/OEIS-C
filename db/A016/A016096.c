/*
 * A016096  a(n+1) = a(n) + sum of its digits, with a(1) = 9.
 * https://oeis.org/A016096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3399, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 9; a(n+1) = a(n) + digit_sum(a(n)) */
/* Memoised implementation – handles arbitrary order of calls. */

#include "../utils/digit_sum.h"
#include "../utils/seq_realloc.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for n=0 – return 0 */
        return (seq_t)0;

    /* static cache for memoisation */
    static idx_t max_cached = 0;     /* largest n computed so far (1‑based) */
    static seq_t *cache = NULL;      /* array of terms, 0‑based */
    static size_t capacity = 0;      /* current allocated capacity */

    /* grow storage if needed */
    if ((size_t)n > capacity) {
        cache = seq_realloc(cache, capacity, (size_t)n);
        capacity = (size_t)n;
    }

    /* compute terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)9;                     /* a(1) */
        } else {
            seq_t prev = cache[(size_t)(i - 2)];      /* a(i-1) */
            seq_t dsum = digit_sum(prev);            /* sum of digits of a(i-1) */
            cache[(size_t)(i - 1)] = prev + dsum;     /* a(i)   */
        }
    }
    if (n > max_cached)
        max_cached = n;

    return cache[(size_t)(n - 1)];
}
