/*
 * A045279  Numbers whose base-5 representation contains no 2's and exactly
 *          two 4's.
 * https://oeis.org/A045279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-5 representation contains no 2's and exactly
 * two 4's.
 * The function builds the n‑th such number by constructing it
 * digit‑by‑digit in lexicographic order.  No lookup table is used. */
#include <stdbool.h>
#include <stdint.h>

/* Computes binomial coefficient C(n,k) for unsigned arguments,
 * using only seq_t arithmetic. */
static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

/* Number of ways to fill R positions with exactly rem4 digits = 4
 * (other positions are 0,1 or 3). Leading zeros are allowed here
 * because the leading digit has already been fixed outside of this
 * function. */
static seq_t count_completions(idx_t R, idx_t rem4) {
    if (rem4 > R) return 0;
    seq_t ways = binom(R, rem4);
    for (idx_t i = 0; i < R - rem4; ++i) ways *= 3;
    return ways;
}

/* Count of admissible numbers of exact length L (leading digit ≠ 0). */
static seq_t count_length(idx_t L) {
    if (L <= 1) return 0;
    seq_t total = 0;

    /* Case 1: leading digit is 4 (uses one of the two 4's). */
    if (L >= 2) {
        seq_t ways = binom(L - 1, 1);          /* choose position for second 4 */
        for (idx_t i = 0; i < L - 2; ++i) ways *= 3;
        total += ways;
    }

    /* Case 2: leading digit is 1 or 3 (the two 4's are in the rest of the
     * number).  There are exactly two possibilities for the leading digit. */
    if (L >= 3) {
        seq_t ways = binom(L - 1, 2);          /* choose two positions for 4 */
        for (idx_t i = 0; i < L - 3; ++i) ways *= 3;
        total += 2 * ways;
    }

    return total;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* outside the sequence; harmless */

    /* Find the decimal position (number of digits) of the n‑th number. */
    idx_t cum = 0;
    idx_t len = 1;
    for (;;) {
        seq_t cnt = count_length(len);
        /* cnt fits in idx_t for the small n we handle. */
        if (cum + (idx_t)cnt >= n) break;
        cum += (idx_t)cnt;
        ++len;
    }

    /* Rank within numbers of this length (1‑based). */
    seq_t rank = (seq_t)(n - cum);

    /* Build the number digit by digit. */
    seq_t value = 0;
    idx_t rem4 = 2;          /* two 4's still to place */

    for (idx_t pos = 0; pos < len; ++pos) {
        /* Candidate digits for this position. */
        int candidates[4];
        int cnt = 0;
        if (pos == 0) {               /* leading digit cannot be 0 */
            candidates[cnt++] = 1;
            candidates[cnt++] = 3;
            candidates[cnt++] = 4;
        } else {
            candidates[cnt++] = 0;
            candidates[cnt++] = 1;
            candidates[cnt++] = 3;
            candidates[cnt++] = 4;
        }

        for (int ci = 0; ci < cnt; ++ci) {
            int d = candidates[ci];
            idx_t next_rem4 = rem4 - (d == 4);
            idx_t remaining = len - pos - 1;

            if (next_rem4 > remaining) continue;   /* not enough positions left */

            seq_t ways = count_completions(remaining, next_rem4);
            if (rank <= ways) {
                value = value * 5 + (seq_t)d;
                rem4 = next_rem4;
                break;
            } else {
                rank -= ways;
            }
        }
    }

    return value;
}
