/*
 * A039087  Numbers whose base-9 representation has the same number of 1's and
 *          5's.
 * https://oeis.org/A039087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9951 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: numbers whose base-9 representation has the same number of 1's and 5's */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return 1 if cand has an equal number of digits 1 and 5 in base‑9 */
static int has_equal_1_5(idx_t cand) {
    seq_t val = cand;
    int c1 = 0, c5 = 0;
    while (val > 0) {
        int d = (int)(val % 9);
        if (d == 1) ++c1;
        else if (d == 5) ++c5;
        val /= 9;
    }
    return c1 == c5;
}

static seq_t *a_cache = NULL;
static size_t a_cap   = 0;
static size_t a_len   = 0;
static idx_t  a_last  = 0;

/* Grow the cache when full */
static void a_expand(void) {
    size_t new_cap = a_cap == 0 ? 64 : a_cap * 2;
    seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
    if (!tmp) exit(1);                 /* out of memory */
    a_cache = tmp;
    a_cap   = new_cap;
}

/* Append a new term to the cache */
static void a_add_candidate(idx_t cand) {
    if (a_len == a_cap) a_expand();
    a_cache[a_len++] = (seq_t)cand;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* guard, offset is 1 */

    /* Initialise cache on first call */
    if (!a_cache) {
        a_expand();
        a_add_candidate(0);           /* a(1) = 0 */
        a_last = 0;
    }

    /* Return already computed term if it exists */
    if (n <= a_len) return a_cache[n - 1];

    /* Generate more terms until we have the nth */
    for (idx_t cand = a_last + 1; ; ++cand) {
        if (has_equal_1_5(cand)) {
            a_add_candidate(cand);
            if (a_len == n) {
                a_last = cand;        /* last processed number */
                break;
            }
        }
        a_last = cand;
    }

    return a_cache[n - 1];
}
