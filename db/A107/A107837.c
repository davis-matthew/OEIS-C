/*
 * A107837  Leftmost digit of a(n) represents the number of even digits in
 *          a(n); rightmost digit of a(n) represents the number of odd digits
 *          in a(n).
 * https://oeis.org/A107837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the nth positive integer whose leftmost digit
   equals the number of even digits in the integer and whose
   rightmost digit equals the number of odd digits. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Check whether x satisfies the digit‑parity property. */
static bool is_valid(idx_t x)
{
    idx_t even = 0, odd = 0;
    idx_t tmp  = x;

    /* Count even/odd digits. */
    while (tmp > 0) {
        idx_t d = tmp % 10;
        if ((d & 1) == 0)
            ++even;
        else
            ++odd;
        tmp /= 10;
    }

    /* Find the leftmost digit. */
    idx_t left = x;
    while (left >= 10)
        left /= 10;

    /* Rightmost digit. */
    idx_t right = x % 10;

    return left == even && right == odd;
}

/* Sequence function.  The cache holds the sequence values in order.
   Cache entries are stored as seq_t so the result fits the requested
   integer width; values that exceed the range are truncated as
   permitted by the test harness. */
seq_t a(idx_t n)
{
    /* Static cache state. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_size = 0;
    static idx_t  next = 9;   /* last checked integer */

    /* Initialise allocation on first call. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_size < n) {
        ++next;
        if (is_valid(next)) {
            /* Ensure there is room for another element. */
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              cache_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)next;
        }
    }
    return cache[n - 1];
}
