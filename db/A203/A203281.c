/*
 * A203281  Number of nX4 binary arrays with every element neighboring
 *          horizontally or vertically both a 0 and a 1.
 * https://oeis.org/A203281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over rows for 4‑column binary arrays with sliding window over
 *            three consecutive rows. The implementation uses a finite state
 *            automaton with 16 states (all 4‑bit patterns). For each triple
 *            of consecutive rows (up,mid,down) we pre‑compute whether the
 *            middle row satisfies the neighbor condition,...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool precomputed = false;
static bool valid_middle[16][16][16];
static bool valid_first[16][16];
static bool valid_last[16][16];

/* Check whether a 4‑bit row cur satisfies the requirement using
   its up and/or down neighbours. */
static bool row_ok(uint16_t cur, bool up_exist, uint16_t up,
                   bool down_exist, uint16_t down)
{
    for (int col = 0; col < 4; ++col) {
        bool has0 = false, has1 = false;

        /* left neighbour */
        if (col > 0) {
            int v = (cur >> (col - 1)) & 1;
            if (v == 0) has0 = true; else has1 = true;
        }

        /* right neighbour */
        if (col < 3) {
            int v = (cur >> (col + 1)) & 1;
            if (v == 0) has0 = true; else has1 = true;
        }

        /* up neighbour */
        if (up_exist) {
            int v = (up >> col) & 1;
            if (v == 0) has0 = true; else has1 = true;
        }

        /* down neighbour */
        if (down_exist) {
            int v = (down >> col) & 1;
            if (v == 0) has0 = true; else has1 = true;
        }

        if (!(has0 && has1))
            return false;   /* this cell fails the condition */
    }
    return true;
}

/* One‑time pre‑computation of valid transitions */
static void init_precompute(void)
{
    for (int up = 0; up < 16; ++up)
        for (int cur = 0; cur < 16; ++cur)
            for (int down = 0; down < 16; ++down)
                valid_middle[up][cur][down] =
                    row_ok((uint16_t)cur, true, (uint16_t)up,
                           true, (uint16_t)down);

    for (int cur = 0; cur < 16; ++cur)
        for (int down = 0; down < 16; ++down)
            valid_first[cur][down] =
                row_ok((uint16_t)cur, false, 0,
                       true, (uint16_t)down);

    for (int up = 0; up < 16; ++up)
        for (int cur = 0; cur < 16; ++cur)
            valid_last[up][cur] =
                row_ok((uint16_t)cur, true, (uint16_t)up,
                       false, 0);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0; /* a(1) = 0 by definition */

    if (!precomputed) {
        init_precompute();
        precomputed = true;
    }

    static seq_t dp[16][16];   /* counts for two previous rows */
    static seq_t ndp[16][16];  /* counts for sliding window */

    /* initialise dp with first two rows */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[i][j] = 0;

    for (int first = 0; first < 16; ++first)
        for (int second = 0; second < 16; ++second)
            if (valid_first[first][second])
                dp[first][second] = (seq_t)1;

    /* iterate rows 3 .. n */
    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                ndp[i][j] = 0;          /* clear next DP */

        for (int up = 0; up < 16; ++up)
            for (int cur = 0; cur < 16; ++cur) {
                seq_t ways = dp[up][cur];
                if (!ways) continue;
                for (int down = 0; down < 16; ++down)
                    if (valid_middle[up][cur][down])
                        ndp[cur][down] += ways;
            }

        /* copy ndp back into dp for next iteration */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final validation of the last row */
    seq_t total = 0;
    for (int up = 0; up < 16; ++up)
        for (int cur = 0; cur < 16; ++cur) {
            seq_t ways = dp[up][cur];
            if (!ways) continue;
            if (valid_last[up][cur])
                total += ways;
        }

    return total;
}
