/*
 * A168001  Numbers k with property that first digit of 7*k = last digit of k.
 * https://oeis.org/A168001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 999, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th integer k such that the first digit of 7*k
 * equals the last digit of k.  The function searches for such k and
 * memoises the results so that each call works in O(n) time. */
#include <stddef.h>
#include <stdlib.h>

/* Return the first (most significant) decimal digit of x. */
static seq_t first_digit(seq_t x)
{
    while (x >= 10)
        x /= 10;
    return x;
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t  capacity  = 0;
    static size_t  length    = 0;
    static idx_t   scanned_k = 0;   /* last k examined */

    /* Grow the cache if needed. */
    if (capacity < n) {
        size_t new_cap = capacity ? capacity * 2 : 128;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure: return 0 */
        for (size_t i = 0; i < length; ++i)
            tmp[i] = cache[i];
        if (cache)
            free(cache);
        cache   = tmp;
        capacity = new_cap;
    }

    /* Find enough terms. */
    while (length < n) {
        ++scanned_k;
        seq_t prod = (seq_t)scanned_k * 7;
        seq_t fd   = first_digit(prod);          /* first digit of 7*k */
        seq_t ld   = (seq_t)(scanned_k % 10);    /* last digit of k */
        if (fd == ld)
            cache[length++] = scanned_k;
    }

    return cache[n - 1];
}
