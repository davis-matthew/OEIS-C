/*
 * A021300  Decimal expansion of 1/296.
 * https://oeis.org/A021300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th digit of the decimal expansion of 1/296
   (offset 0, a(0) = 0, a(1) = 0, a(2) = 3, …). */

#include <stdlib.h>   /* malloc, realloc */

static seq_t  *cache   = NULL;   /* cached digits */
static idx_t   cache_len = 0;    /* next free index (one past the last stored) */
static size_t  cache_cap = 0;    /* allocated capacity */
static seq_t   rem       = (seq_t)1;   /* remainder after last computed digit */

/* Return the n-th digit of 1/296. */
seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Ensure capacity */
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing digits up to n using long division */
    for (idx_t i = cache_len; i <= n; ++i) {
        rem = rem * (seq_t)10;
        seq_t digit = rem / (seq_t)296;
        rem      %= (seq_t)296;
        cache[i] = digit;
    }
    cache_len = n + 1;   /* update count of stored digits */

    return cache[n];
}
