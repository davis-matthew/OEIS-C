/*
 * A083441  Largest n-digit palindrome with a digit sum of n.
 * https://oeis.org/A083441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = largest n‑digit palindrome with digit sum n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following algorithm builds the palindrome greedily: */
/*  - choose the leftmost digit as large as possible while the rest of the */
/*    digits can still realise the required sum. */
/*  - for each pair of symmetric positions (except possibly the middle), */
/*    subtract 2×digit from the remaining sum; for the middle digit subtract */
/*    digit. */
/*  - the resulting digits are then assembled into a seq_t value. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used, but safe */

    idx_t half = n / 2;
    bool odd   = (n & 1) != 0;

    unsigned char *dig = (unsigned char *)malloc(n * sizeof *dig);
    if (!dig) return 0;                 /* allocation failure */

    seq_t rem = (seq_t)n;                /* remaining sum to distribute */

    for (idx_t pos = 0; pos < half; ++pos) {
        unsigned char min_d = (pos == 0) ? 1 : 0;   /* leading digit cannot be 0 */
        for (int cand = 9; cand >= min_d; --cand) {
            seq_t two_d = (seq_t)(cand * 2);
            if (two_d > rem) continue;            /* cannot subtract more than we have */

            seq_t new_rem = rem - two_d;

            idx_t remaining_pairs = half - pos - 1;
            seq_t max_rest = (seq_t)(remaining_pairs * 18);
            if (odd) max_rest += 9;

            if (new_rem <= max_rest) {             /* feasible */
                dig[pos] = (unsigned char)cand;
                dig[n - 1 - pos] = (unsigned char)cand;
                rem = new_rem;
                break;
            }
        }
    }

    if (odd) {                                   /* middle digit */
        int mid_val = -1;
        for (int cand = 9; cand >= 0; --cand) {
            if (rem == (seq_t)cand) { mid_val = cand; break; }
        }
        if (mid_val < 0) mid_val = 0;              /* safety, should not happen */
        dig[half] = (unsigned char)mid_val;
        rem = 0;
    }

    /* construct numeric value from the digit array */
    seq_t result = 0;
    for (idx_t i = 0; i < n; ++i) {
        result = result * 10 + dig[i];
    }

    free(dig);
    return result;
}
