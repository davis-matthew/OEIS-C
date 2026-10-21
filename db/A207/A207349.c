/*
 * A207349  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of 5×n binary matrices with no * horizontal
 *            000 or 011 in any row and no vertical 000 or 111 in any column.
 *            * Computed by dynamic programming on column states
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 19, u64 n <= 45, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=16, a(2)=256. */

#include <stdint.h>
#include <stdbool.h>

#define ROWS 5
#define STATE_BITS ROWS
#define MAX_STATE (1 << STATE_BITS)   /* 32 */

/* pre‑computed tables (built on first call) */
static bool init_done = false;
static bool vert_ok[MAX_STATE];
static bool horiz_ok[MAX_STATE][MAX_STATE][MAX_STATE];
static seq_t count_allowed;

/* initialise lookup tables */
static void init_tables(void)
{
    if (init_done) return;

    /* vertical validity: forbid 000 and 111 in any 3 consecutive rows */
    for (int mask = 0; mask < MAX_STATE; ++mask) {
        bool ok = true;
        for (int r = 0; r <= ROWS - 3 && ok; ++r) {
            int b0 = (mask >> r)       & 1;
            int b1 = (mask >> (r + 1)) & 1;
            int b2 = (mask >> (r + 2)) & 1;
            if (b0 == b1 && b1 == b2) ok = false;
        }
        vert_ok[mask] = ok;
    }

    /* count how many single columns are allowed */
    seq_t cnt = 0;
    for (int mask = 0; mask < MAX_STATE; ++mask)
        if (vert_ok[mask]) ++cnt;
    count_allowed = cnt;

    /* horizontal constraints over any three consecutive columns
     * Forbidden patterns in a row: 000 and 011
     */
    for (int a = 0; a < MAX_STATE; ++a)
        for (int b = 0; b < MAX_STATE; ++b)
            for (int c = 0; c < MAX_STATE; ++c) {
                bool ok = true;
                for (int r = 0; r < ROWS && ok; ++r) {
                    int da = (a >> r) & 1;
                    int db = (b >> r) & 1;
                    int dc = (c >> r) & 1;
                    if ((da == 0 && db == 0 && dc == 0) ||
                        (da == 0 && db == 1 && dc == 1))
                        ok = false;
                }
                horiz_ok[a][b][c] = ok;
            }

    init_done = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_tables();

    /* base cases */
    if (n == 0) return 0;                /* out of sequence range */
    if (n == 1) return count_allowed;    /* one column */
    if (n == 2) return count_allowed * count_allowed;

    /* dynamic programming over pairs of adjacent columns */
    const int STATE_COUNT = MAX_STATE * MAX_STATE;  /* 1024 */

    seq_t dp[STATE_COUNT];
    seq_t next[STATE_COUNT];

    /* initialise dp with all allowed first two columns */
    for (int i = 0; i < STATE_COUNT; ++i) dp[i] = 0;
    for (int prev = 0; prev < MAX_STATE; ++prev)
        if (vert_ok[prev])
            for (int curr = 0; curr < MAX_STATE; ++curr)
                if (vert_ok[curr])
                    dp[(prev << STATE_BITS) | curr] = 1;

    /* extend column by column */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;

        for (int idx = 0; idx < STATE_COUNT; ++idx) {
            seq_t val = dp[idx];
            if (!val) continue;
            int prev = idx >> STATE_BITS;
            int curr = idx & (MAX_STATE - 1);

            for (int newcol = 0; newcol < MAX_STATE; ++newcol) {
                if (!vert_ok[newcol]) continue;
                if (!horiz_ok[prev][curr][newcol]) continue;
                int idx2 = (curr << STATE_BITS) | newcol;
                next[idx2] = next[idx2] + val;
            }
        }
        /* swap dp and next for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = next[i];
    }

    /* sum all final states to obtain the total count */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total = total + dp[i];
    return total;
}
