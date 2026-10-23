/*
 * A036447  Double and reverse digits.
 * https://oeis.org/A036447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n+1) = reverse_decimal(2*a(n)), a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoization to handle arbitrary call order.
 * A small helper reverses decimal digits of a seq_t value
 * without relying on external utilities.
 */

#include <stdlib.h>

/* Helper: reverse decimal digits of a seq_t number */
static seq_t reverse_digits_seq(seq_t x)
{
    seq_t rev = 0;
    while (x != 0) {
        seq_t digit = x % 10;
        rev = rev * 10 + digit;
        x = x / 10;
    }
    return rev;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure cache is initialized with base case a(0) = 1 */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* Extend cache up to index n if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;

        for (size_t i = cache_len; i < new_len; ++i) {
            /* a(i) = reverse_decimal(2 * a(i-1)) */
            seq_t doubled = cache[i - 1] * 2;
            cache[i] = reverse_digits_seq(doubled);
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
