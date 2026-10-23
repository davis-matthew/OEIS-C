/*
 * A043505  Numbers having one 4 in base 10.
 * https://oeis.org/A043505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Sequence: A043505. Numbers having exactly one digit 4 in decimal. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static seq_t next_test = (seq_t)0;

    /* Ensure the cache contains at least n terms. */
    while (cache_len < n) {
        /* Grow the cache if it is full. */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? (cache_cap << 1) : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* Allocation failed. */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Look for the next number containing exactly one digit 4. */
        ++next_test;
        seq_t t = next_test;
        int count4 = 0;
        while (t > (seq_t)0) {
            if ((t % (seq_t)10) == (seq_t)4)
                ++count4;
            t /= (seq_t)10;
        }
        if (count4 == 1)
            cache[cache_len++] = next_test;
    }

    return cache[n - 1];   /* Return the requested term (1‑based index). */
}
