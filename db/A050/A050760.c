/*
 * A050760  Triangular numbers whose decimal expansion contains no pair of
 *          consecutive equal digits.
 * https://oeis.org/A050760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 265, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 986 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050760:
   a(n) = the n-th triangular number T_k = k*(k+1)/2 that has no pair of
   consecutive equal decimal digits.  Formally,
   a(n) = T_k where k is the n-th integer ≥ 0 satisfying
          no_consecutive_equal_digits(T_k). */

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

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Undefined for n < 1; return 0 as a safe fallback. */
        return (seq_t)0;
    }

    idx_t k = 0;          /* Current candidate for the triangular index. */
    idx_t found = 0;      /* Number of qualifying triangular numbers found. */

    while (true) {
        seq_t val = ((seq_t)k * ((seq_t)k + (seq_t)1)) / (seq_t)2;
        if (no_consecutive_equal_digits(val)) {
            found++;
            if (found == n) {
                return val;
            }
        }
        k++;
    }
}
