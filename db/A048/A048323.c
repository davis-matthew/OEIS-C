/*
 * A048323  Numbers whose base-12 expansions, read from left to right, have
 *          run lengths that strictly decrease.
 * https://oeis.org/A048323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th non-negative integer whose base‑12 expansion has
 *            strictly decreasing run lengths
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_decreasing_runs(seq_t v)
{
    /* Special case v = 0 */
    if (v == (seq_t)0) return true;

    /* Store digits of v in base‑12 in reverse order */
    int digits[64];
    int dcnt = 0;
    seq_t tmp = v;
    while (tmp > (seq_t)0) {
        digits[dcnt++] = (int)(tmp % (seq_t)12);
        tmp /= (seq_t)12;
    }
    if (dcnt == 0) {  /* v == 0 already handled, but keeps compiler happy */
        digits[0] = 0;
        dcnt = 1;
    }

    int i = dcnt - 1;           /* index of most significant digit */
    int prev_len = -1;          /* length of previous run; -1 for none */

    while (i >= 0) {
        int cur_digit = digits[i];
        int run_len = 1;
        /* Count consecutive equal digits to the left */
        i--;
        while (i >= 0 && digits[i] == cur_digit) {
            run_len++;
            i--;
        }
        if (prev_len != -1 && run_len >= prev_len) {
            return false;      /* run length does not strictly decrease */
        }
        prev_len = run_len;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t val = 0;

    while (1) {
        if (has_decreasing_runs(val)) {
            if (seen == n) return val;
            seen++;
        }
        val++;
    }
}
