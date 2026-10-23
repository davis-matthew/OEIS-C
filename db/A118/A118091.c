/*
 * A118091  Start with 1 and repeatedly reverse the digits and add 46 to get
 *          the next term.
 * https://oeis.org/A118091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = reverse_digits(a(n-1)) + 46 for n > 1 */

#include <stdlib.h>

/* Reverse the decimal digits of a value of type seq_t. */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    const seq_t ZERO = (seq_t)0;
    while (x != ZERO) {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x = x / 10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* Cache of previously computed terms.  cache[0] holds a(1). */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;  /* number of elements stored */

    if (n == 0) return (seq_t)0;  /* undefined behaviour for n < 1 */

    /* Ensure the cache is large enough to hold up to n terms. */
    if (cache_len < (size_t)n) {
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap : 1;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;  /* Allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms. */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;               /* a(1) */
            } else {
                seq_t prev = cache[i - 1];
                seq_t rev  = reverse_digits(prev);
                cache[i]   = rev + (seq_t)46;
            }
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
