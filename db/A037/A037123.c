/*
 * A037123  a(n) = a(n-1) + sum of digits of n.
 * https://oeis.org/A037123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + sum of decimal digits of n, a(0) = 0
 * Range:     u8 n <= 42, u16 n <= 4386, u32 n <= 9895, u64 n <= 9895,
 *            u128 n <= 9895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the decimal digit sum of an idx_t value */
static inline seq_t digit_sum_idx(idx_t v)
{
    seq_t sum = 0;
    while (v) {
        sum += v % 10;
        v /= 10;
    }
    return sum;
}

/* Cached values of the sequence */
static seq_t *a_cache = NULL;
static idx_t   a_len   = 0;  /* number of cached entries (size) */

/* Return the n-th term of OEIS A037123 */
seq_t a(idx_t n)
{
    /* Ensure the cache starts with a(0) = 0 */
    if (a_len == 0) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();           /* allocation failed */
        a_cache[0] = 0;
        a_len = 1;
    }

    /* Extend cache up to n if necessary */
    if (n + 1 > a_len) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        a_cache = tmp;

        /* Compute new terms */
        for (idx_t i = a_len; i <= n; ++i) {
            seq_t ds = digit_sum_idx(i);
            a_cache[i] = a_cache[i - 1] + ds;
        }
        a_len = new_size;
    }

    return a_cache[n];
}
