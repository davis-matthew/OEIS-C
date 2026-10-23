/*
 * A174931  Half of the digital sum of base 3 representations of 2^n.
 * https://oeis.org/A174931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (sum of base‑3 digits of 2^n) / 2
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A174931 */

#include <stdlib.h>

/* Compute a(n) for any n ≥ 1.
   The function is memoised across calls; it works for any seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* Static data persistent across calls */
    static unsigned char *digits   = NULL;  /* base‑3 digits of 2^k, least significant first */
    static size_t          digits_len = 0;   /* number of digits                      */
    static idx_t          current_k   = 0;   /* largest k whose 2^k is cached          */

    static seq_t *cache = NULL;             /* 1‑based cache of a(k) values           */
    static size_t   cache_cap = 0;          /* allocated capacity of 'cache'          */
    static size_t   cache_len = 0;          /* number of cached entries               */

    /* Initialise on first call */
    if (cache_len == 0) {
        /* 2^1 = 2 in base 3 → digits[0] = 2 */
        digits_len = 1;
        digits     = (unsigned char *)malloc(digits_len * sizeof(unsigned char));
        digits[0]  = 2;

        /* Cache a(1) = 2/2 = 1 */
        cache_cap = 1;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = (seq_t)1;
        cache_len = 1;

        current_k = 1;
    }

    /* Handle the trivial case where n is within the cached range */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Extend the cache up to n */
    for (idx_t k = current_k; k < n; ++k) {
        /* Multiply current base‑3 number by 2 */
        unsigned int carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            unsigned int val = (unsigned int)digits[i] * 2 + carry;
            digits[i] = val % 3;
            carry     = val / 3;
        }
        while (carry > 0) {
            /* Allocate space for a new digit */
            ++digits_len;
            digits = (unsigned char *)realloc(digits,
                                            digits_len * sizeof(unsigned char));
            digits[digits_len - 1] = carry % 3;
            carry /= 3;
        }

        /* Sum the digits in a wide integer to avoid overflow */
        unsigned long long sum = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            sum += (unsigned long long)digits[i];
        }

        /* Grow the cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *new_cache = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
            cache     = new_cache;
            cache_cap = new_cap;
        }

        /* Store a(k+1) = sum/2 */
        cache[cache_len++] = (seq_t)(sum / 2);
        ++current_k;  /* now 2^k is cached */
    }

    return cache[n - 1];
}
