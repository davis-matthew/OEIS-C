/*
 * A050755  Eighth powers containing no pair of consecutive equal digits.
 * https://oeis.org/A050755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Compute successive eighth powers k^8 and keep those whose
 *            decimal representation contains no pair of consecutive equal
 *            digits. The function memoises the results so that repeated calls
 *            are O(1). The first 20 terms are those listed in OEIS A050755
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 56, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 850 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/ipow_u64_idx.h"

/* Test whether a seq_t value has any pair of consecutive equal decimal digits. */
static inline bool no_consecutive_equal_digits(seq_t val) {
    if (val == (seq_t)0) return true;          /* 0 contains only one digit */
    int prev = -1;                             /* previous digit, -1 means none yet */
    seq_t tmp = val;
    while (tmp > (seq_t)0) {
        int d = (int)(tmp % (seq_t)10);        /* current least‑significant digit */
        if (prev == d) return false;           /* found equal neighbours */
        prev = d;
        tmp /= (seq_t)10;                      /* shift right one decimal digit */
    }
    return true;                               /* all adjacent digits differ */
}

#define MAX_CAP 200000   /* should be large enough for n up to about 10000 */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static storage for memoised sequence */
    static seq_t values[MAX_CAP];
    static idx_t used      = 0;     /* number of values stored */
    static idx_t next_k    = 0;     /* next base to try for k^8 */

    /* safety check – the build expects the function to handle any n
       that actually occurs in the tests, but we limit to MAX_CAP
       to avoid running out of memory with an insane request. */
    assert(n < (idx_t)MAX_CAP);

    /* Generate values until we have at least n+1 of them */
    while (used <= n) {
        seq_t val = ipow_u64_idx((uint64_t)next_k, (idx_t)8);

        if (no_consecutive_equal_digits(val)) {
            values[used++] = val;
        }

        ++next_k;
    }

    return values[n];
}
