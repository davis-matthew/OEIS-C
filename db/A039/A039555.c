/*
 * A039555  Numbers whose base-12 representation has the same nonzero number
 *          of 8's and 11's.
 * https://oeis.org/A039555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th positive integer whose base‑12 representation
   contains an equal (and non‑zero) number of digits 8 and 11. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if x, written in base‑12, has the same non‑zero numbers
   of digits 8 and 11. */
static bool has_equal_8_11(seq_t x)
{
    unsigned long long cnt8 = 0;
    unsigned long long cnt11 = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        seq_t d = t % (seq_t)12;
        if (d == (seq_t)8)        cnt8++;
        else if (d == (seq_t)11)  cnt11++;
        t = t / (seq_t)12;
    }
    return (cnt8 > (unsigned long long)0) && (cnt8 == cnt11);
}

/* Return the n-th element of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *cache    = NULL;   /* array of terms */
    static size_t   cap    = 0;      /* allocated capacity */
    static size_t   len    = 0;      /* number of terms cached */
    static seq_t   next_candidate = (seq_t)1; /* next integer to test */

    /* Already have the term? */
    if (len >= (size_t)n) {
        return cache[(size_t)n - 1U];
    }

    /* Grow the cache if needed. */
    if (len == cap) {
        size_t newcap = (cap == 0U) ? 128U : cap * 2U;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        if (!tmp) { /* allocation failure – return 0 as sentinel */
            return (seq_t)0;
        }
        cache = tmp;
        cap   = newcap;
    }

    /* Search for new terms until we reach index n. */
    seq_t cur = next_candidate;
    for (;;) {
        if (has_equal_8_11(cur)) {
            cache[len++] = cur;
            if (len == (size_t)n) {        /* found the required term */
                next_candidate = cur + (seq_t)1;
                return cur;
            }
        }
        next_candidate = cur + (seq_t)1;
        ++cur;
    }
}
