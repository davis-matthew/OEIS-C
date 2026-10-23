/*
 * A043855  Numbers k such that number of runs in the base 4 representation of
 *          k is congruent to 5 mod 8.
 * https://oeis.org/A043855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th integer k such that the number of runs in the
   base‑4 representation of k is congruent to 5 (mod 8). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cached values of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* capacity of the cache array */
    static idx_t next = 1;        /* next candidate to test */

    if (n == 0) return 0;          /* n is always >= 1 in the OEIS offset */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t temp = next;
        int prev_digit = -1;
        int runs = 0;

        /* Count runs in the base‑4 representation of 'next' */
        while (temp > 0) {
            int digit = (int)(temp % 4);
            if (digit != prev_digit) {
                ++runs;
                prev_digit = digit;
            }
            temp /= 4;
        }

        if (runs % 8 == 5) {
            /* Ensure enough capacity and store the term */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next;
        }

        ++next;
    }

    return cache[n - 1];
}
