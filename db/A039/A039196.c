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
 * Helpers:   same5and7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039196: numbers whose base‑11 representation has the same
   number of 5's and 7's.
   The function builds the sequence lazily and memoises the results
   so that subsequent calls with arbitrary indices are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/same5and7.h"

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
            seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
            seq_len = 0;
            next_val = 0;
        }

        /* grow storage if needed */
        if (seq_cap < n) {
            seq_cap = n * 2;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
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
