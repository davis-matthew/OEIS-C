/*
 * A039196  Numbers whose base-11 representation has the same number of 5's
 *          and 7's.
 * https://oeis.org/A039196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039196: numbers whose base‑11 representation has the same
   number of 5's and 7's.
   The function builds the sequence lazily and memoises the results
   so that subsequent calls with arbitrary indices are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* local helper – true when the base‑11 representation of v
   contains an equal number of digit 5 and digit 7 */
static bool same5and7(seq_t v) {
    int cnt5 = 0;
    int cnt7 = 0;
    seq_t tmp = v;
    do {
        seq_t d = tmp % (seq_t)11;   /* 11 fits in any seq_t */
        if (d == (seq_t)5) cnt5++;
        else if (d == (seq_t)7) cnt7++;
        tmp /= (seq_t)11;
    } while (tmp > (seq_t)0);
    return cnt5 == cnt7;
}

/* memoised sequence storage */
static seq_t *seq = NULL;
static size_t seq_len = 0;    /* # of terms stored (always ≥ 1 once initialised) */
static size_t seq_cap = 0;    /* allocated capacity */
static idx_t next_val = 0;    /* next integer to test (as idx_t) */

seq_t a(idx_t n) {
    /* n ≥ 1 here (offset=1) */
    if (seq_len < (size_t)n) {
        /* initialise storage if not yet done */
        if (seq_cap == 0) {
            seq_cap = 32;
            seq = mem_new(seq_t, seq_cap);
            seq_len = 0;
            next_val = 0;
        }

        /* grow storage if needed */
        if (seq_cap < n) {
            seq_cap = n * 2;
            seq = mem_resize(seq_t, seq, seq_cap);
        }

        /* generate terms until we have n of them */
        while (seq_len < (size_t)n) {
            seq_t val = (seq_t)next_val;
            if (same5and7(val)) {
                seq[seq_len++] = val;
            }
            ++next_val;
        }
    }
    return seq[n - 1];
}
