/*
 * A044954  Numbers with no two equally numerous base 5 digits.
 * https://oeis.org/A044954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   digit_base5, unique_nonzero_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th positive integer whose base‑5 digits have
   all distinct non‑zero frequencies (no two digits occur the same number
   of times).  The code builds the sequence incrementally by testing
   successive integers. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_base5.h"
#include "../utils/unique_nonzero_counts.h"

/* Maximum number of sequence terms that can be stored.
   For n ≤ 10 000 the values stay well below 50 000, so 20 000 is safe. */
#define MAX_TERMS 20000

/* Static storage for memoised terms */
static seq_t seq_storage[MAX_TERMS];
static size_t seq_len = 0;    /* number of terms already found */
static seq_t next_candidate = 1;  /* next integer to test */

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1; 0 is unused */

    /* Generate terms until we have at least n of them */
    while (n > seq_len) {
        if (seq_len >= MAX_TERMS) {
            /* In the unlikely event we exceed the static buffer, just
               continue producing values without storing them. */
            if (unique_nonzero_counts(next_candidate))
                seq_len++;   /* increment count to keep n check */
        } else {
            if (unique_nonzero_counts(next_candidate))
                seq_storage[seq_len++] = next_candidate;
        }
        next_candidate++;
    }
    return seq_storage[n - 1];
}
