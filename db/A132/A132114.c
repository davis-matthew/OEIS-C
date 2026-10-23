/*
 * A132114  Multiply previous term by 7 and reverse.
 * https://oeis.org/A132114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1; a(n) = reverse_decimal(7 * a(n-1)) for n > 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Reverse the decimal digits of a seq_t value.
   Works for any integer type that supports % and /. */
static seq_t reverse_decimal(seq_t x) {
    seq_t rev = 0;
    while (x != 0) {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x = x / 10;
    }
    return rev;
}

/* Cached sequence array.
   cache[0] holds a(1), cache[1] holds a(2), …  */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_len = 0;    /* number of terms stored (>=1) */

/* Public function implementing the OEIS sequence A132114. */
seq_t a(idx_t n) {
    if (n == 0) {
        /* Sequence is defined for n >= 1 only. Return 0 for safety. */
        return 0;
    }

    /* Initialize cache for the first call. */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) {
            /* Allocation failure; fallback to zero. */
            return 0;
        }
        cache[0] = 1;        /* a(1) = 1 */
        cache_len = 1;
    }

    /* Grow cache if needed */
    if ((idx_t)n > (idx_t)cache_cap) {
        size_t new_cap = cache_cap;
        while ((idx_t)new_cap < n)
            new_cap *= 2;          /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) {
            /* Allocation failed; return last cached value. */
            return cache[cache_len - 1];
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t tmp = 7 * cache[i - 1];
        cache[i] = reverse_decimal(tmp);
    }
    cache_len = n;

    return cache[n - 1];
}
