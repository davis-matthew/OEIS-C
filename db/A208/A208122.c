/*
 * A208122  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=20, a(2)=400 * Recurrence: For n≥3, let col[0..19] be the
 *            20 binary strings of length 7 * that avoid the vertical patterns
 *            001 and 011. * If pair (p,q) is two consecutive columns, the
 *            third column r is admissible * iff for every row r:
 *            (col[p][row],col[q][row],col[r][row]) is neither 000 nor...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* pre‑computed data */
    static bool initialized = false;
    static uint8_t cols[20];
    static int next_cnt[20][20];
    static int next_list[20][20][20];
    static seq_t dp_cur[400];
    static seq_t dp_nxt[400];

    /* first run: build the list of allowed columns and adjacency graph */
    if (!initialized) {
        /* build cols */
        int k = 0;
        for (int mask = 0; mask < 128; ++mask) {
            bool ok = true;
            for (int r = 0; r <= 4; ++r) {
                int b0 = (mask >> r) & 1;
                int b1 = (mask >> (r + 1)) & 1;
                int b2 = (mask >> (r + 2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 0 && b1 == 1 && b2 == 1)) {
                    ok = false;
                    break;
                }
            }
            if (ok) cols[k++] = (uint8_t)mask;
        }
        /* k should be 20 */

        /* build adjacency list: next[-] */
        for (int p = 0; p < 20; ++p)
            for (int q = 0; q < 20; ++q) {
                int cnt = 0;
                for (int r = 0; r < 20; ++r) {
                    bool ok = true;
                    for (int row = 0; row < 7 && ok; ++row) {
                        int a = (cols[p] >> row) & 1;
                        int b = (cols[q] >> row) & 1;
                        int c = (cols[r] >> row) & 1;
                        if ((a == 0 && b == 0 && c == 0) ||
                            (a == 0 && b == 1 && c == 0))
                            ok = false;
                    }
                    if (ok) next_list[p][q][cnt++] = r;
                }
                next_cnt[p][q] = cnt;
            }
        initialized = true;
    }

    /* base cases */
    if (n == 0) return 0;          /* not used, but safe */
    if (n == 1) return 20;
    if (n == 2) return 400;

    const size_t STATES = 400;     /* 20 * 20 */

    /* initialize DP for two columns */
    for (size_t i = 0; i < STATES; ++i)
        dp_cur[i] = 1;

    /* iterate columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        for (size_t i = 0; i < STATES; ++i)
            dp_nxt[i] = 0;

        for (size_t idx = 0; idx < STATES; ++idx) {
            seq_t val = dp_cur[idx];
            if (val == 0) continue;
            uint8_t p = (uint8_t)(idx / 20);
            uint8_t q = (uint8_t)(idx % 20);
            int cnt = next_cnt[p][q];
            for (int t = 0; t < cnt; ++t) {
                int r = next_list[p][q][t];
                size_t nxt_idx = ((size_t)q) * 20 + r;
                dp_nxt[nxt_idx] += val;
            }
        }
        /* swap dp_cur and dp_nxt */
        for (size_t i = 0; i < STATES; ++i)
            dp_cur[i] = dp_nxt[i];
    }

    /* sum all counts for last two columns */
    seq_t total = 0;
    for (size_t i = 0; i < STATES; ++i)
        total += dp_cur[i];
    return total;
}
