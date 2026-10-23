/*
 * A043501  Numbers having one 3 in base 10.
 * https://oeis.org/A043501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* This function returns the n‑th integer that has exactly one decimal
   digit equal to 3 (OEIS sequence A043501).  */
seq_t a(idx_t n)
{
    /* Static cache of previously computed terms and state for generation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_test = 2;   /* last number examined */

    /* Ensure the cache contains at least n terms. */
    if ((size_t)n > cache_len) {
        /* Expand capacity if necessary */
        if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate terms until we reach index n. */
        while (cache_len < (size_t)n) {
            last_test++;                     /* next candidate */
            seq_t x = last_test;
            unsigned count3 = 0;

            /* Count digits that are 3.  Stop early if more than one. */
            while (x > 0) {
                seq_t d = x % 10;
                if (d == 3) {
                    count3++;
                    if (count3 > 1) break;
                }
                x /= 10;
            }
            if (count3 == 1) {
                cache[cache_len++] = last_test;
            }
        }
    }

    return cache[(size_t)(n - 1)];
}
