/*
 * A206932  Number of nX4 0..1 arrays avoiding 0 0 0 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming: a(1)=13, a(2)=169 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static bool init_done = false;
static bool allowed_row[16];
static uint16_t next_mask[256];
static seq_t a1_val, a2_val;

static void init_tables(void)
{
    if (init_done) return;
    init_done = true;

    /* Determine which single rows are horizontally allowed */
    for (int r = 0; r < 16; ++r) {
        bool left_three_zero  = ((r & 0x07) == 0);   /* bits 0,1,2 all zero */
        bool right_three_zero = ((r & 0x0E) == 0);   /* bits 1,2,3 all zero */
        allowed_row[r] = !(left_three_zero || right_three_zero);
    }

    /* Count allowed rows (a(1)) and compute a(2)=a1^2 */
    a1_val = (seq_t)0;
    for (int r = 0; r < 16; ++r)
        if (allowed_row[r]) a1_val += (seq_t)1;
    a2_val = a1_val * a1_val;

    /* Pre‑compute allowed transitions for each state (prev2<<4 | prev1) */
    for (int s = 0; s < 256; ++s) {
        int prev2 = s >> 4;
        int prev1 = s & 0x0F;
        uint16_t mask = 0;
        for (int r = 0; r < 16; ++r) {
            if (!allowed_row[r]) continue;
            bool ok = true;
            for (int col = 0; col < 4; ++col) {
                int b1 = (prev2 >> col) & 1;
                int b2 = (prev1 >> col) & 1;
                int b3 = (r      >> col) & 1;
                if (b1 == 0 && b2 == 1 && b3 == 1) { ok = false; break; }
            }
            if (ok) mask |= (1u << r);
        }
        next_mask[s] = mask;
    }
}

extern seq_t a(idx_t n)
{
    init_tables();
    if (n == 0) return (seq_t)0;
    if (n == 1) return a1_val;
    if (n == 2) return a2_val;

    static seq_t dp[256];
    static seq_t ndp[256];
    for (int i = 0; i < 256; ++i) dp[i] = (seq_t)0;

    /* Initialise for the first two rows (any allowed pair) */
    for (int r1 = 0; r1 < 16; ++r1) if (allowed_row[r1])
        for (int r2 = 0; r2 < 16; ++r2) if (allowed_row[r2]) {
            int state = (r1 << 4) | r2;
            dp[state] = (seq_t)1;
        }

    /* Process rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 256; ++i) ndp[i] = (seq_t)0;
        for (int s = 0; s < 256; ++s) {
            seq_t cnt = dp[s];
            if (cnt == (seq_t)0) continue;
            uint16_t mask = next_mask[s];
            for (int r = 0; r < 16; ++r) {
                if (mask & (1u << r)) {
                    int new_state = ((s & 0x0F) << 4) | r;
                    ndp[new_state] += cnt;
                }
            }
        }
        for (int i = 0; i < 256; ++i) dp[i] = ndp[i];
    }

    seq_t total = (seq_t)0;
    for (int i = 0; i < 256; ++i) total += dp[i];
    return total;
}
