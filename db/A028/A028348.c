/*
 * A028348  Numbers k such that k^2 + 3*k + 1 is a palindrome.
 * https://oeis.org/A028348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 28, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028348: numbers k such that k^2 + 3*k + 1 is a palindrome in base 10. */
/* The function builds an internal cache of the terms; any order of calls is handled. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;      /* cached k values */
    static size_t cache_len = 0;       /* number of terms stored so far */
    static size_t cache_cap = 0;       /* current capacity of the cache array */

    /* Return the value if it is already cached. */
    if (n <= (idx_t)cache_len)
        return cache[(size_t)n - 1];

    /* Start searching at the smallest integer not yet examined. */
    seq_t k = (cache_len > 0) ? cache[cache_len - 1] + 1 : (seq_t)1;

    /* Find enough terms to satisfy the request. */
    while (cache_len < n) {
        /* Evaluate k^2 + 3*k + 1. */
        seq_t f = k * k + (seq_t)3 * k + (seq_t)1;

        /* Check if the result is a decimal palindrome. */
        if (is_palindrome(f, (idx_t)10)) {
            /* Resize the cache if necessary. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)   /* allocation failed – return 0 as sentinel */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the found value. */
            cache[cache_len++] = k;
        }
        ++k;
    }

    return cache[(size_t)n - 1];
}
