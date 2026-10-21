/*
 * A066711  RATS: Reverse Add Then Sort the digits applied to previous term,
 *          starting with 9.
 * https://oeis.org/A066711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 9; for n > 1, a(n) = sort_digits_inc( a(n−1) +
 *            reverse_digits(a(n−1)) ) where sort_digits_inc( ) sorts the
 *            decimal digits of its argument into increasing order (leading
 *            zeros are discarded)
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   sort_digits_inc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sort_digits_inc.h"

/* Reverse the decimal digits of x. */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > 0) {
        seq_t d = x % 10;
        rev = rev * 10 + d;
        x   = x / 10;
    }
    return rev;
}

/* One step of the sequence: from current term to the next. */
static seq_t next_term(seq_t cur)
{
    seq_t rev = reverse_digits(cur);
    return sort_digits_inc(cur + rev);
}

/* Memoised computation of the sequence. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)9;

    /* Static cache of computed terms: cache[i] holds a(i+1). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* current capacity */

    /* Initialise cache with a(1)=9 if needed. */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) /* allocation failed */
            return (seq_t)0;
        cache[0] = (seq_t)9;
        cache_len = 1;
    }

    /* Return a previously computed term if possible. */
    if (n <= cache_len)
        return cache[n-1];

    /* Compute missing terms up to n, extending the cache. */
    seq_t curr = cache[cache_len - 1];
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t nxt = next_term(curr);
        /* Grow cache if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) /* allocation failed */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len] = nxt;
        cache_len++;
        curr = nxt;
    }

    return cache[n-1];
}
