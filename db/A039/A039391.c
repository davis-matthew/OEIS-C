/*
 * A039391  Numbers whose base-9 representation has the same nonzero number of
 *          6's and 8's.
 * https://oeis.org/A039391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7663, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force scanning of integers checking base‑9 digits
   for equal nonzero counts of 6s and 8s. */
#include <stdbool.h>
#include "../utils/append_candidate.h"

/* Checks whether a number has the same non‑zero number of digits 6
   and 8 in its base‑9 representation. */
static bool good(seq_t x)
{
    if (x == 0) return false;
    int c6 = 0, c8 = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        int d = (int)(tmp % 9);
        if (d == 6) ++c6;
        else if (d == 8) ++c8;
        tmp /= 9;
    }
    return c6 == c8 && c6 > 0;
}

seq_t a(idx_t n)
{
    /* cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 1;         /* next integer to test */

    while (cache_len < n) {
        /* find next qualifying integer */
        while (!good(next)) ++next;
        append_candidate(&cache, &cache_len, &cache_cap, next);
        ++next;
    }
    return cache[n - 1];
}
