/*
 * A044954  Numbers with no two equally numerous base 5 digits.
 * https://oeis.org/A044954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th positive integer whose base‑5 digits have
   all distinct non‑zero frequencies (no two digits occur the same number
   of times).  The code builds the sequence incrementally by testing
   successive integers. */

#include <stdbool.h>
#include <stddef.h>

/* Return the base‑5 digit (0‑4) of v as an unsigned int without
   converting directly from seq_t to a builtin type. */
static inline unsigned digit_base5(seq_t v) {
    seq_t r = v % 5;          /* remainder when divided by 5 */
    if (r == 0) return 0;
    if (r == 1) return 1;
    if (r == 2) return 2;
    if (r == 3) return 3;
    /* r must be 4 */
    return 4;
}

/* Check if 'v' has distinct non‑zero digit counts in base 5. */
static bool unique_nonzero_counts(seq_t v) {
    unsigned char cnt[5] = {0};
    while (v > 0) {
        unsigned d = digit_base5(v);
        cnt[d]++;
        v = v / 5;
    }
    for (int i = 0; i < 5; ++i) {
        if (!cnt[i]) continue;
        for (int j = i + 1; j < 5; ++j) {
            if (!cnt[j]) continue;
            if (cnt[i] == cnt[j]) return false;
        }
    }
    return true;
}

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
