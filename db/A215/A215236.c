/*
 * A215236  Greatest integer k such that n^i has no identical consecutive
 *          digits for i = 0..k.
 * https://oeis.org/A215236
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A217157(n)-1 = greatest k such that n^i has no
 *            consecutive identical digits for i = 0..k
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 103 terms through n = 9811 (2026-11-01)
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

seq_t a(idx_t n) {
    /* n >= 2 (offset 2) – the function ignores lower indices. */
    seq_t val = (seq_t)1;      /* n^0 */
    idx_t i = 0;               /* current exponent */

    while (1) {
        /* If the current power contains consecutive equal digits, the
           first failure occurs at exponent i, so the answer is i-1. */
        if (!no_consecutive_equal_digits(val))
            return (seq_t)(i - 1);

        /* Advance to the next power.  The helper works with the
           underlying precision of seq_t (either built‑in or arbitrary
           precision). */
        val = val * ((seq_t)n);
        ++i;
    }
}
