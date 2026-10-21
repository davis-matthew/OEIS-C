/*
 * A053059  Squares whose product of digits is also a nonzero square.
 * https://oeis.org/A053059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A053059 – Squares whose product of digits is a non‑zero square
 * Algorithm: iterate over increasing integers k, test k²: if the digit
 * product (ignoring zeros) is a perfect square, store it.
 * Cache results so that any order of calls is fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

/* Return true iff the product of the decimal digits of x is a non‑zero square. */
static inline bool digit_prod_is_square(seq_t x)
{
    const seq_t ten = (seq_t)10;
    seq_t prod = (seq_t)1;

    while (x != (seq_t)0) {
        seq_t d = x % ten;
        if (d == (seq_t)0)          /* contains a zero digit => product zero */
            return false;
        prod *= d;
        x /= ten;
    }

    seq_t s = floor_sqrt(prod);
    return s * s == prod;
}

/* a(n) = the nth square whose digit product is a non‑zero perfect square. */
seq_t a(idx_t n)
{
    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;      /* number of terms stored */
    static idx_t last_k   = 0;       /* last integer whose square was tested */

    /* Undefined for n == 0 – return 0 as a safe guard. */
    if (n == 0)
        return (seq_t)0;

    /* If we already have the value cached, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure enough space in the cache. */
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Search until we have found n terms. */
    for (;;) {
        ++last_k;
        seq_t sq = (seq_t)last_k * (seq_t)last_k;
        if (digit_prod_is_square(sq)) {
            /* Expand cache if needed. */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = sq;
            if (cache_len == n)
                return sq;
        }
    }
}
