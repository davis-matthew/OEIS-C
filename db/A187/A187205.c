/*
 * A187205  Numbers such that the last of the absolute differences of divisors
 *          is 0.
 * https://oeis.org/A187205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 910 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187205: Numbers such that the last of the absolute differences
   of their sorted divisors is 0.  The function a(n) returns the n‑th such
   number (1‑based).  The value is computed by brute‑force search using
   an efficient divisor enumeration and iterative difference reduction. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: check if n has the property that repeated absolute differences
   of its sorted divisors result in 0.  */
static bool has_zero_div_diff(uint64_t n)
{
    /* Collect divisors in ascending order efficiently. */
    idx_t small[256], large[256];
    size_t sc = 0, lc = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            small[sc++] = d;
            if (d * d != n) large[lc++] = n / d;
        }
    }

    /* Concatenate small in order, large in reverse order. */
    idx_t seq[512];
    size_t len = 0;
    for (size_t i = 0; i < sc; ++i) seq[len++] = small[i];
    for (size_t i = lc; i > 0; --i) seq[len++] = large[i - 1];

    /* Apply the successive difference operation until one element remains. */
    while (len > 1) {
        size_t newlen = len - 1;
        for (size_t i = 0; i < newlen; ++i) {
            idx_t a = seq[i], b = seq[i + 1];
            seq[i] = (a > b) ? (a - b) : (b - a);
        }
        len = newlen;
    }
    return seq[0] == 0;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for computed terms, grows as needed. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t cnt   = 0;
    static uint64_t next = 1;

    /* Grow cache and search forward until we have at least n terms. */
    while (cnt < n) {
        /* Find next qualifying number. */
        while (!has_zero_div_diff(next)) ++next;

        /* Append to cache, resizing if necessary. */
        if (cnt >= cap) {
            size_t newcap = cap ? cap * 2 : 16;
            seq_t *tmp    = mem_resize(seq_t, cache, newcap);
            if (!tmp) abort();           /* allocation failure */
            cache = tmp;
            cap   = newcap;
        }
        cache[cnt++] = (seq_t)next;
        ++next;   /* advance candidate for next iteration */
    }

    return cache[n - 1];
}
