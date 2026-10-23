/*
 * A188905  Number of nX4 binary arrays without the pattern 0 1 0 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* A188905 – number of n×4 binary arrays avoiding the pattern 0 1 0 on any diagonal
 * or anti‑diagonal.  Computed by a DP over the last two rows. */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)256;

    /* Pre‑compute allowed transitions from a pair of rows to the next row. */
    static bool allowed[256][16] = {0};
    static bool init = false;
    if (!init) {
        for (int a = 0; a < 16; ++a) {
            for (int b = 0; b < 16; ++b) {
                int state = a * 16 + b;
                for (int c = 0; c < 16; ++c) {
                    bool ok = true;
                    /* diagonal j = 0 : a[0], b[1], c[2] */
                    if (((a >> 0) & 1) == 0 && ((b >> 1) & 1) == 1 &&
                        ((c >> 2) & 1) == 0) ok = false;
                    /* diagonal j = 1 : a[1], b[2], c[3] */
                    if (((a >> 1) & 1) == 0 && ((b >> 2) & 1) == 1 &&
                        ((c >> 3) & 1) == 0) ok = false;
                    /* anti‑diagonal j = 0 : a[2], b[1], c[0] */
                    if (((a >> 2) & 1) == 0 && ((b >> 1) & 1) == 1 &&
                        ((c >> 0) & 1) == 0) ok = false;
                    /* anti‑diagonal j = 1 : a[3], b[2], c[1] */
                    if (((a >> 3) & 1) == 0 && ((b >> 2) & 1) == 1 &&
                        ((c >> 1) & 1) == 0) ok = false;
                    allowed[state][c] = ok;
                }
            }
        }
        init = true;
    }

    seq_t dp_prev[256];
    seq_t dp_cur[256];

    /* For the first two rows every pair of rows is admissible. */
    for (int i = 0; i < 256; ++i) dp_prev[i] = (seq_t)1;

    /* Extend the array row by row. */
    for (idx_t cur = 3; cur <= n; ++cur) {
        for (int i = 0; i < 256; ++i) dp_cur[i] = (seq_t)0;

        for (int prev = 0; prev < 256; ++prev) {
            seq_t cnt = dp_prev[prev];
            if (cnt == (seq_t)0) continue;
            int prev1 = prev % 16;          /* second row of the stored pair */
            for (int nxt = 0; nxt < 16; ++nxt) {
                if (!allowed[prev][nxt]) continue;
                int new_state = prev1 * 16 + nxt;
                dp_cur[new_state] += cnt;
            }
        }

        /* Prepare for the next iteration. */
        for (int i = 0; i < 256; ++i) dp_prev[i] = dp_cur[i];
    }

    /* The answer is the sum over all states for the last row. */
    seq_t answer = (seq_t)0;
    for (int i = 0; i < 256; ++i) answer += dp_prev[i];
    return answer;
}
