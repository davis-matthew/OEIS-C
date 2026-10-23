/*
 * A033936  a(n+1) = a(n) + sum of squares of digits of a(n).
 * https://oeis.org/A033936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; a(n+1) = a(n) + sum_{d in digits of a(n)} d^2
 * Range:     u8 n <= 9, u16 n <= 565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Return the sum of the squares of the decimal digits of x. */
static inline seq_t digit_square_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        seq_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

/* Compute the n-th element of A033936.  The function is memoised
   and works for calls in any order. */
seq_t a(idx_t n)
{
    /* initialise cache with first term */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
    }

    /* if requested term already cached, return it */
    if (n < cache_cap)
        return cache[n];

    /* grow cache to cover all terms up to n */
    idx_t new_cap = n + 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    for (idx_t i = cache_cap; i < new_cap; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = prev + digit_square_sum(prev);
    }
    cache_cap = new_cap;

    return cache[n];
}
