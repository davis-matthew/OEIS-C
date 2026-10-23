/*
 * A044300  Numbers k such that string 5,4 occurs in the base 9 representation
 *          of k but not of k-1.
 * https://oeis.org/A044300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that base-9 representation of k contains "54"
   but base-9 representation of k-1 does not. */

#include <stdlib.h>

static int has54(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int cur = (int)(x % 9);
        if (prev == 4 && cur == 5)
            return 1;
        prev = cur;
        x /= 9;
    }
    return 0;
}

seq_t a(idx_t n)
{
    /* Cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t current_k = 1;           /* next candidate to test */

    /* Grow cache as needed */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate until we have at least n terms */
    while (cache_len < n) {
        if (has54(current_k)) {
            seq_t prev_k = current_k - 1;
            if (!has54(prev_k)) {
                cache[cache_len++] = current_k;
            }
        }
        ++current_k;
    }

    return cache[n - 1];
}
