/*
 * A072227  Numbers n such that reverse(d) divides reverse(n) for all divisors
 *          d of n.
 * https://oeis.org/A072227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1752, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A072227:
   numbers n such that reverse(d) divides reverse(n) for all divisors d of n.
   The implementation uses a linear search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/rev.h"

/* Test whether x satisfies the definition:
   for every divisor d of x, reverse(d) divides reverse(x). */
static bool good_candidate(idx_t x)
{
    seq_t rev_x = (seq_t)rev(x);          /* reverse of n */
    for (idx_t d = 1; d * d <= x; ++d) {
        if (x % d == 0) {
            seq_t rev_d = (seq_t)rev(d);
            if (rev_x % rev_d != 0)
                return false;
            idx_t d2 = x / d;
            if (d2 != d) {
                seq_t rev_d2 = (seq_t)rev(d2);
                if (rev_x % rev_d2 != 0)
                    return false;
            }
        }
    }
    return true;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    /* Static cache of found terms (1‑based). */
    static seq_t *cache = NULL;
    static size_t c_len = 0;   /* number of terms stored */
    static size_t c_cap = 0;   /* allocated capacity */
    static idx_t last_checked = 0;  /* largest integer already examined */

    /* Grow the cache until we have at least n terms. */
    while (c_len < n) {
        ++last_checked;
        if (good_candidate(last_checked)) {
            /* Ensure capacity. */
            if (c_len == c_cap) {
                c_cap = c_cap ? c_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, c_cap * sizeof(seq_t));
                cache = tmp;
            }
            cache[c_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
