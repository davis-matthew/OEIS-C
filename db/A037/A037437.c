/*
 * A037437  Positive numbers having the same set of digits in base 6 and base
 *          10.
 * https://oeis.org/A037437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 211, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9368 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Positive numbers having the same set of digits in base 10 and base 6.
   a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 5, ...        */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Determine whether a positive integer x has the same set of digits
   in base 10 and base 6. */
static bool same_digits_set(idx_t x)
{
    bool dec[10] = {0};
    bool six[6] = {0};
    idx_t t;

    /* decimal digits */
    t = x;
    while (t > 0) {
        idx_t d = t % 10;
        if (d > 5)                          /* a decimal digit >5 cannot appear in base 6 */
            return false;
        dec[d] = true;
        t /= 10;
    }

    /* base‑6 digits */
    t = x;
    while (t > 0) {
        idx_t d = t % 6;
        six[d] = true;
        t /= 6;
    }

    /* compare the sets up to digit 5 (digits 6–9 are already known to be absent in dec) */
    for (idx_t d = 0; d < 6; ++d)
        if (dec[d] != six[d])
            return false;

    return true;
}

/* Returns the n‑th element of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored terms */
    static idx_t cache_len = 0;      /* number of stored terms */
    static idx_t cache_cap = 0;      /* allocated capacity of cache */
    static idx_t next_search = 1;    /* next candidate integer to test */

    if (n == 0)
        return 0;                    /* off‑by‑offset behaviour is irrelevant */

    /* ensure we have computed at least the first n terms */
    while (cache_len < n) {
        /* extend cache if needed */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return 0;      /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* test candidates until we find the next term */
        while (!same_digits_set(next_search))
            ++next_search;

        cache[cache_len++] = (seq_t)next_search;
        ++next_search;                /* advance for next search */
    }

    return cache[n - 1];
}
