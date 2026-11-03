/*
 * A050752  Fifth powers containing no pair of consecutive equal digits.
 * https://oeis.org/A050752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth fifth power of an integer whose decimal
 *            representation has no consecutive equal digits. Computed by a
 *            brute‑force search
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

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

/* Compute k^5 for an integer k of type seq_t. */
static inline seq_t pow5(seq_t k) {
    seq_t r = k;
    r *= k;  /* k^2   */
    r *= k;  /* k^3   */
    r *= k;  /* k^4   */
    r *= k;  /* k^5   */
    return r;
}

/* Return the nth member of sequence A050752 (offset 0). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* 0^5 = 0, first term */

    idx_t found = 0;                           /* count of qualifying powers seen so far */
    for (idx_t k = 1;; ++k) {
        seq_t power = pow5((seq_t)k);          /* compute k^5 */
        if (no_consecutive_equal_digits(power)) {
            ++found;
            if (found == n) return power;     /* nth qualifying fifth power found */
        }
    }
}
