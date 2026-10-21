/*
 * A203340  Number of nX2 0..4 arrays with every nonzero element less than or
 *          equal to at least two horizontal and vertical neighbors.
 * https://oeis.org/A203340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×2 arrays with elements 0..4 satisfying the
 *            condition: every non‑zero element is ≤ at least two of its
 *            horizontal and vertical neighbors
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   digit_from_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_from_row.h"

static inline bool row_ok(int prev_row, bool prev_exists,
                          int curr_row, int next_row, bool next_exists) {
    /* Check the constraint for the current row, given the previous and
       next rows (which may be absent). */
    int a = digit_from_row(curr_row, 0);  /* left cell */
    int b = digit_from_row(curr_row, 1);  /* right cell */

    if (a > 0) {
        int cnt = 0;
        if (prev_exists && digit_from_row(prev_row, 0) >= a) cnt++;
        if (next_exists && digit_from_row(next_row, 0) >= a) cnt++;
        if (digit_from_row(curr_row, 1) >= a) cnt++;
        if (cnt < 2) return false;
    }

    if (b > 0) {
        int cnt = 0;
        if (prev_exists && digit_from_row(prev_row, 1) >= b) cnt++;
        if (next_exists && digit_from_row(next_row, 1) >= b) cnt++;
        if (digit_from_row(curr_row, 0) >= b) cnt++;
        if (cnt < 2) return false;
    }

    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* safety, not used by OEIS */
    if (n == 1) return (seq_t)1;          /* only the all‑zero row */

    /* Precompute allowed transitions for the interior rows:
       T[prev][curr][next] is true iff the middle row "curr" is valid
       when it is surrounded by prev and next rows. */
    static bool initialized = false;
    static bool allowed_next[625][25];
    if (!initialized) {
        for (int s = 0; s < 625; ++s) {
            int prev = s / 25;
            int curr = s % 25;
            for (int nxt = 0; nxt < 25; ++nxt) {
                allowed_next[s][nxt] = row_ok(prev, true, curr, nxt, true);
            }
        }
        initialized = true;
    }

    /* DP arrays: dp_cur[state] = number of ways for the last two rows
       corresponding to 'state'.  State is encoded as prev*25 + curr. */
    static seq_t dp_cur[625];
    static seq_t dp_next[625];

    /* Clear current dp */
    for (int i = 0; i < 625; ++i) dp_cur[i] = (seq_t)0;

    /* Initialize with the first two rows, verifying the first row. */
    for (int r1 = 0; r1 < 25; ++r1) {
        for (int r2 = 0; r2 < 25; ++r2) {
            if (row_ok(-1, false, r1, r2, true))
                dp_cur[r1 * 25 + r2] = dp_cur[r1 * 25 + r2] + (seq_t)1;
        }
    }

    /* Process rows 3 .. n, using the precomputed transitions. */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 625; ++i) dp_next[i] = (seq_t)0;

        for (int s = 0; s < 625; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            int curr = s % 25;
            for (int nxt = 0; nxt < 25; ++nxt) {
                if (!allowed_next[s][nxt]) continue;
                int idx = curr * 25 + nxt;   /* new state: (curr, nxt) */
                dp_next[idx] = dp_next[idx] + val;
            }
        }

        /* Copy next to current for next iteration. */
        for (int i = 0; i < 625; ++i) dp_cur[i] = dp_next[i];
    }

    /* Final validation: the last row must satisfy its boundary condition. */
    seq_t ans = (seq_t)0;
    for (int s = 0; s < 625; ++s) {
        seq_t val = dp_cur[s];
        if (val == (seq_t)0) continue;
        int prev = s / 25;
        int curr = s % 25;
        if (row_ok(prev, true, curr, -1, false))
            ans = ans + val;
    }

    return ans;
}
