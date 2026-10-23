/*
 * A039547  Numbers whose base-12 representation has the same nonzero number
 *          of 6's and 10's.
 * https://oeis.org/A039547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th positive integer whose base‑12 representation
 * contains an equal non‑zero number of digits 6 and 10.
 * The implementation searches the integers sequentially. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_checked = 0;

    if (n <= cache_len)
        return cache[n - 1];

    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    idx_t x;
    for (x = last_checked + 1; cache_len < n; ++x) {
        idx_t v = x;
        unsigned cnt6 = 0, cnt10 = 0;
        while (v > 0) {
            uint8_t d = (uint8_t)(v % 12);
            if (d == 6) ++cnt6;
            else if (d == 10) ++cnt10;
            v /= 12;
        }
        if (cnt6 == cnt10 && cnt6 > 0) {
            cache[cache_len++] = (seq_t)x;
        }
    }
    last_checked = x - 1;

    return cache[n - 1];
}
