/*
 * A207690  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207690
   Number of n×5 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
   and 0 0 1 and 1 1 1 vertically.
   Computed by a transfer‑matrix dynamic programming over rows. */

#include <stdint.h>
#include <stdbool.h>

static bool init_computed = false;
static bool allowed_row[32];
static uint32_t next_mask[32][32];
static int  allowed_rows[32];
static int  allowed_count;

/* test whether a 5‑bit pattern satisfies the horizontal constraints
   (no 000 or 010) */
static bool row_ok(int m) {
    for (int i = 0; i <= 2; ++i) {
        int b0 = (m >> i)     & 1;
        int b1 = (m >> (i+1)) & 1;
        int b2 = (m >> (i+2)) & 1;
        if (b0==0 && b1==0 && b2==0) return false;
        if (b0==0 && b1==1 && b2==0) return false;
    }
    return true;
}

/* test whether a triple of rows satisfies the vertical constraints
   (no 001 or 111 in any column) */
static bool vertical_ok(int a, int b, int c) {
    for (int col = 0; col < 5; ++col) {
        int r0 = (a >> col) & 1;
        int r1 = (b >> col) & 1;
        int r2 = (c >> col) & 1;
        if (r0==0 && r1==0 && r2==1) return false;
        if (r0==1 && r1==1 && r2==1) return false;
    }
    return true;
}

/* first time the function is called, compute the constant tables */
static void init_tables(void) {
    if (init_computed) return;
    allowed_count = 0;
    for (int m = 0; m < 32; ++m) {
        allowed_row[m] = row_ok(m);
        if (allowed_row[m]) {
            allowed_rows[allowed_count++] = m;
        }
    }
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            uint32_t mask = 0;
            for (int c = 0; c < 32; ++c) {
                if (allowed_row[c] && vertical_ok(a,b,c)) {
                    mask |= (1u << c);
                }
            }
            next_mask[a][b] = mask;
        }
    }
    init_computed = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    init_tables();

    if (n == 0) return 0;                 /* undefined offset below 1 */
    if (n == 1) return (seq_t)allowed_count;
    if (n == 2) return (seq_t)allowed_count * (seq_t)allowed_count;

    /* DP arrays: dp_curr[prev][curr] holds counts for last two rows */
    seq_t dp_curr[32][32] = {0};
    seq_t dp_next[32][32] = {0};

    /* initialise for first two rows */
    for (int r1 = 0; r1 < 32; ++r1) if (allowed_row[r1]) {
        for (int r2 = 0; r2 < 32; ++r2) if (allowed_row[r2]) {
            dp_curr[r1][r2] = 1;
        }
    }

    /* add rows 3 .. n */
    for (int step = 3; step <= (int)n; ++step) {
        /* clear dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        /* transition */
        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t val = dp_curr[prev][curr];
                if (!val) continue;
                uint32_t mask = next_mask[prev][curr];
                for (int nxt = 0; nxt < 32; ++nxt) {
                    if (mask & (1u << nxt)) {
                        dp_next[curr][nxt] += val;
                    }
                }
            }
        }
        /* copy dp_next to dp_curr for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_curr[i][j] = dp_next[i][j];
    }

    /* sum all final states */
    seq_t result = 0;
    for (int prev = 0; prev < 32; ++prev)
        for (int curr = 0; curr < 32; ++curr)
            result += dp_curr[prev][curr];

    return result;
}
