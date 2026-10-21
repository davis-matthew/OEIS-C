/*
 * A229369  Number of nX5 0..2 arrays avoiding 11 horizontally, 22 vertically
 *          and 00 antidiagonally.
 * https://oeis.org/A229369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of n×5 arrays over digits {0,1,2} avoiding 11
 *            horizontally, 22 vertically, and 00 anti–diagonally
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define STATE_COUNT 243

/* Pre‑computed state data and transition lists – built once. */
static bool init_done       = false;
static bool valid_state[STATE_COUNT];
static unsigned char digits[STATE_COUNT][5];
static int prev_lists[STATE_COUNT][STATE_COUNT];
static int prev_counts[STATE_COUNT];

static void init(void)
{
    if (init_done) return;

    /* Generate digit arrays for all 3^5 states and filter horizontal 11. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int t = s;
        for (int col = 4; col >= 0; --col) {
            digits[s][col] = t % 3;
            t /= 3;
        }
        bool ok = true;
        for (int col = 0; col < 4 && ok; ++col)
            if (digits[s][col] == 1 && digits[s][col + 1] == 1)
                ok = false;
        valid_state[s] = ok;
    }

    /* Build lists of compatible previous states for each current state. */
    for (int cur = 0; cur < STATE_COUNT; ++cur) {
        if (!valid_state[cur]) continue;
        int cnt = 0;
        for (int prev = 0; prev < STATE_COUNT; ++prev) {
            if (!valid_state[prev]) continue;
            bool ok = true;
            for (int col = 0; col < 5 && ok; ++col)
                if (digits[prev][col] == 2 && digits[cur][col] == 2)
                    ok = false;
            for (int col = 1; col < 5 && ok; ++col)
                if (digits[prev][col] == 0 && digits[cur][col - 1] == 0)
                    ok = false;
            if (ok)
                prev_lists[cur][cnt++] = prev;
        }
        prev_counts[cur] = cnt;
    }
    init_done = true;
}

seq_t a(idx_t n)
{
    init();

    static seq_t dp_prev[STATE_COUNT];
    static seq_t dp_cur[STATE_COUNT];
    for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = 0;
    for (int s = 0; s < STATE_COUNT; ++s)
        if (valid_state[s]) dp_prev[s] = 1;

    if (n == 1) {
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s)
            if (valid_state[s]) total += dp_prev[s];
        return total;
    }

    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;
        for (int cur = 0; cur < STATE_COUNT; ++cur) {
            if (!valid_state[cur]) continue;
            seq_t sum = 0;
            int cnt = prev_counts[cur];
            for (int k = 0; k < cnt; ++k) {
                int prev = prev_lists[cur][k];
                sum += dp_prev[prev];
            }
            dp_cur[cur] = sum;
        }
        for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = dp_cur[i];
    }

    seq_t total = 0;
    for (int s = 0; s < STATE_COUNT; ++s)
        if (valid_state[s]) total += dp_prev[s];
    return total;
}
