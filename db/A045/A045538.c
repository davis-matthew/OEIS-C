/*
 * A045538  Smallest number k such that kn contains two consecutive digits
 *          which are the same.
 * https://oeis.org/A045538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = minimal positive k such that the decimal representation
 *            of n*k contains two consecutive equal digits. The implementation
 *            performs a simple linear search from k = 1 upwards
 * Range:     u8 n <= 20, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
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

seq_t a(idx_t n)
{
    if (n == 0) { return 0; }          /* 0 is not defined in the sequence */
    seq_t nseq = (seq_t)n;

    for (seq_t k = 1;; ++k) {
        seq_t prod = nseq * k;
        if (!no_consecutive_equal_digits(prod))
            return k;
    }
}
