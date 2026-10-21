/*
 * A207491  Number of 6 X n 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts 6×n binary matrices avoiding horizontal 000 or 011,
 *            and vertical 011 or 110
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 19, u64 n <= 45, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ROWS 6
#define MAX_COLS 64
#define MAX_STATES (MAX_COLS * MAX_COLS)
#define MAX_NEXT   MAX_COLS

/* Precomputed data, built on first call */
static bool    init = false;
static bool    allowed[MAX_COLS];
static uint8_t next_masks[MAX_STATES][MAX_NEXT];
static uint8_t next_cnt   [MAX_STATES];
static uint8_t allowed_rows[MAX_COLS];
static uint32_t allowed_count;

/* Build tables only once                                 */
static void build_tables(void) {
    /* Determine allowed single‑column masks (vertical patterns) */
    for (uint32_t m = 0; m < MAX_COLS; ++m) {
        bool ok = true;
        for (int r = 0; r <= MAX_ROWS - 3 && ok; ++r) {
            int b0 = (m >> r)     & 1;
            int b1 = (m >> (r+1)) & 1;
            int b2 = (m >> (r+2)) & 1;
            if ((b0 == 0 && b1 == 1 && b2 == 1) ||
                (b0 == 1 && b1 == 1 && b2 == 0)) {
                ok = false;
            }
        }
        allowed[m] = ok;
        if (ok) {
            allowed_rows[m] = 1;
            ++allowed_count;
        } else {
            allowed_rows[m] = 0;
        }
    }

    /* Build transitions for pairs of columns (p1,p2) -> list of p3 */
    for (uint32_t p1 = 0; p1 < MAX_COLS; ++p1) {
        if (!allowed[p1]) continue;
        for (uint32_t p2 = 0; p2 < MAX_COLS; ++p2) {
            if (!allowed[p2]) continue;
            uint32_t idx = (p1 << 6) | p2;
            uint8_t cnt = 0;
            for (uint32_t p3 = 0; p3 < MAX_COLS; ++p3) {
                if (!allowed[p3]) continue;
                bool good = true;
                for (int r = 0; r < MAX_ROWS && good; ++r) {
                    int b0 = (p1 >> r) & 1;
                    int b1 = (p2 >> r) & 1;
                    int b2 = (p3 >> r) & 1;
                    if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                        (b0 == 0 && b1 == 1 && b2 == 1)) {
                        good = false;
                    }
                }
                if (good) {
                    next_masks[idx][cnt++] = (uint8_t)p3;
                }
            }
            next_cnt[idx] = cnt;
        }
    }
    init = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset starts at 1 */
    if (!init) build_tables();          /* build tables on first call */

    /* Handle small n directly */
    if (n == 1) return (seq_t)allowed_count;
    if (n == 2) return (seq_t)allowed_count * (seq_t)allowed_count;

    /* DP over columns, keeping last two columns */
    static seq_t dp[MAX_STATES];
    static seq_t next_dp[MAX_STATES];
    memset(dp, 0, sizeof(dp));
    memset(next_dp, 0, sizeof(next_dp));

    /* Initialize for n = 2 (all allowed pairs count as 1) */
    for (uint32_t p1 = 0; p1 < MAX_COLS; ++p1) {
        if (!allowed[p1]) continue;
        for (uint32_t p2 = 0; p2 < MAX_COLS; ++p2) {
            if (!allowed[p2]) continue;
            dp[(p1 << 6) | p2] = 1;
        }
    }

    /* Step through columns 3 … n */
    for (idx_t col = 3; col <= n; ++col) {
        for (uint32_t idx = 0; idx < MAX_STATES; ++idx) {
            if (dp[idx] == 0) continue;
            uint8_t p2 = idx & 0x3F;
            seq_t ways = dp[idx];
            uint8_t cnt = next_cnt[idx];
            for (uint8_t k = 0; k < cnt; ++k) {
                uint8_t p3 = next_masks[idx][k];
                uint32_t next_idx = (p2 << 6) | p3;
                next_dp[next_idx] += ways;
            }
        }
        memcpy(dp, next_dp, sizeof(dp));
        memset(next_dp, 0, sizeof(next_dp));
    }

    /* Sum all possibilities */
    seq_t total = 0;
    for (uint32_t idx = 0; idx < MAX_STATES; ++idx) {
        total += dp[idx];
    }
    return total;
}
