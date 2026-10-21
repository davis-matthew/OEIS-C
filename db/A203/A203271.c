/*
 * A203271  Number of nX2 0..5 arrays with every nonzero element less than or
 *          equal to some NW, E or S neighbor.
 * https://oeis.org/A203271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×2 arrays with digits 0..5 such that every
 *            non–zero entry has at least one neighbor to the north–west, east
 *            or south whose value is ≥ the entry. Computed by dynamic
 *            programming over rows
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define VALS 6                      /* digits 0..5 */
#define STATES (VALS * VALS * 4)    /* left,right,pl,pr */

/* Encode a state: left (0..5), right (0..5), pl (need South for left),
   pr (need South for right).  */
static inline int encode_state(int left, int right, int pl, int pr) {
    return ((left * VALS + right) << 2) | ((pl << 1) | pr);
}

/* Decode a state into its components.  */
static inline void decode_state(int idx, int *left, int *right,
                                int *pl, int *pr) {
    int base = idx >> 2;
    *pl = (idx >> 1) & 1;
    *pr = idx & 1;
    *left  = base / VALS;
    *right = base % VALS;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but guard just in case */

    seq_t dp[STATES] = {0};
    seq_t dp_next[STATES] = {0};

    /* Initial row (row 1).  NW neighbor for column 2 does not exist → pending
       if right > 0.  */
    for (int left = 0; left < VALS; ++left) {
        for (int right = 0; right < VALS; ++right) {
            int pl = (left > 0 && left > right) ? 1 : 0;   /* left satisfied by right? */
            int pr = (right > 0) ? 1 : 0;                  /* no NW neighbor */
            int idx = encode_state(left, right, pl, pr);
            dp[idx] = 1;
        }
    }

    /* Process subsequent rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (int idx = 0; idx < STATES; ++idx) {
            if (dp[idx] == 0) continue;

            int prev_left, prev_right, pl_prev, pr_prev;
            decode_state(idx, &prev_left, &prev_right, &pl_prev, &pr_prev);

            for (int cl = 0; cl < VALS; ++cl) {
                if (pl_prev && cl < prev_left) continue;   /* need South for left */
                for (int cr = 0; cr < VALS; ++cr) {
                    if (pr_prev && cr < prev_right) continue; /* need South for right */

                    int nl = (cl > 0 && cl > cr) ? 1 : 0;          /* left satisfied by right? */
                    int nr = (cr > 0 && cr > prev_left) ? 1 : 0; /* right satisfied by NW? */
                    int idx_next = encode_state(cl, cr, nl, nr);
                    dp_next[idx_next] += dp[idx];
                }
            }
        }

        /* swap dp and dp_next */
        for (int i = 0; i < STATES; ++i) dp[i] = dp_next[i];
    }

    /* Sum only states where last row has no pending South constraints */
    seq_t result = 0;
    for (int idx = 0; idx < STATES; ++idx) {
        int pl, pr;
        /* pl, pr are in bits 1 and 0 of the state */
        if (((idx >> 1) & 1) == 0 && (idx & 1) == 0)
            result += dp[idx];
    }

    return result;
}
