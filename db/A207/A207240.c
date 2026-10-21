/*
 * A207240  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A207240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..1 n×6 arrays avoiding * horizontally: 000
 *            and 011 * vertically: 001 and 010
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* there are only 64 possible rows; or 18 of them satisfy the
     * horizontal constraints.  We pre‑compute these rows and the
     * vertical compatibility between any two consecutive pairs. */
    static const uint8_t COLS = 6;
    static const uint8_t ROWS  = 1U << COLS;        /* 64 */
    static idx_t           allowed_cnt = 0;
    static uint8_t         allowed[18];            /* allowed rows (bit masks) */
    static uint32_t        followMask[18][18];    /* bitmask of compatible
                                                     next rows for each pair
                                                     (a,b) -> a=prev2, b=prev1 */
    static bool            initialized = false;

    if (!initialized) {
        /* ---- horizontal filtering ---- */
        for (uint8_t r = 0; r < ROWS; ++r) {
            bool ok = true;
            for (int i = 0; i <= 3 && ok; ++i) {
                uint8_t block = (r >> i) & 0x07;    /* three consecutive bits */
                if (block == 0x00 || block == 0x03) /* 000 or 011 */
                    ok = false;
            }
            if (ok) {
                allowed[allowed_cnt++] = r;
            }
        }

        /* ---- vertical compatibility ---- */
        for (idx_t a = 0; a < allowed_cnt; ++a) {
            uint8_t r_a = allowed[a];
            for (idx_t b = 0; b < allowed_cnt; ++b) {
                uint8_t r_b = allowed[b];
                uint32_t mask = 0;
                for (idx_t c = 0; c < allowed_cnt; ++c) {
                    uint8_t r_c = allowed[c];
                    bool ok = true;
                    for (int col = 0; col < COLS && ok; ++col) {
                        int top = (r_a >> col) & 1;
                        int mid = (r_b >> col) & 1;
                        int bot = (r_c >> col) & 1;
                        if (top == 0 && mid == 0 && bot == 1) ok = false;
                        if (top == 0 && mid == 1 && bot == 0) ok = false;
                    }
                    if (ok)
                        mask |= (1U << c);
                }
                followMask[a][b] = mask;
            }
        }
        initialized = true;
    }

    /* trivial cases */
    if (n == 0) return 0;                     /* not used by OEIS */
    if (n == 1) return (seq_t)allowed_cnt;     /* 18 */
    if (n == 2) {                             /* 18*18 */
        seq_t s = (seq_t)allowed_cnt;
        return s * s;
    }

    /* ----- dynamic programming for n >= 3 ----- */
    static seq_t dp[18][18];
    static seq_t ndp[18][18];

    /* initialise with first two rows (no vertical restriction yet) */
    for (idx_t a = 0; a < allowed_cnt; ++a)
        for (idx_t b = 0; b < allowed_cnt; ++b)
            dp[a][b] = 1;

    for (idx_t row = 3; row <= n; ++row) {
        /* zero ndp */
        for (idx_t a = 0; a < allowed_cnt; ++a)
            for (idx_t b = 0; b < allowed_cnt; ++b)
                ndp[a][b] = 0;

        /* transition */
        for (idx_t a = 0; a < allowed_cnt; ++a) {
            for (idx_t b = 0; b < allowed_cnt; ++b) {
                seq_t val = dp[a][b];
                if (val == 0) continue;
                uint32_t mask = followMask[a][b];
                for (idx_t c = 0; c < allowed_cnt; ++c) {
                    if ((mask >> c) & 1U) {
                        ndp[b][c] += val;
                    }
                }
            }
        }
        /* copy ndp to dp for next iteration */
        for (idx_t a = 0; a < allowed_cnt; ++a)
            for (idx_t b = 0; b < allowed_cnt; ++b)
                dp[a][b] = ndp[a][b];
    }

    /* sum all end states */
    seq_t total = 0;
    for (idx_t a = 0; a < allowed_cnt; ++a)
        for (idx_t b = 0; b < allowed_cnt; ++b)
            total += dp[a][b];

    return total;
}
