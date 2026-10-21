/*
 * A061810  Multiples of 3 with all odd digits.
 * https://oeis.org/A061810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 884, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over integers, selecting those with only odd
   decimal digits that are multiples of 3.  The terms are stored in a
   static cache so that calls with arbitrary order of n work correctly. */
#include <stdlib.h>
#include <stdbool.h>

static bool all_odd_digits_seq_t(seq_t x) {
    if (x == 0) return false;          /* 0 is not a term of the sequence */
    while (x) {
        seq_t d = x % (seq_t)10;       /* last digit */
        if ((d & 1) == 0)             /* even digit found */
            return false;
        x /= (seq_t)10;                /* drop last digit */
    }
    return true;
}

seq_t a(idx_t n) {
    /* Static cache to hold computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t candidate = (seq_t)3;  /* first candidate to test */

    if (len < n) {
        if (!cache) {
            cap = 16;
            cache = (seq_t*)malloc(cap * sizeof(seq_t));
        }
        while (len < n) {
            /* Advance candidate until it satisfies the conditions */
            while (!all_odd_digits_seq_t(candidate) ||
                   (candidate % (seq_t)3 != 0)) {
                candidate++;
            }

            /* Grow cache if needed */
            if (len == cap) {
                cap *= 2;
                cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
            }

            cache[len++] = candidate;   /* store found term */

            candidate++;                /* next integer for subsequent search */
        }
    }

    return cache[n - 1];
}
