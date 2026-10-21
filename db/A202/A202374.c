/*
 * A202374  Number of (n+2)X6 binary arrays avoiding patterns 001 and 111 in
 *          rows and columns.
 * https://oeis.org/A202374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×6 binary arrays avoiding 001 and 111 in
 *            both rows and columns. The function computes the term using a
 *            transfer-matrix dynamic programming. It is valid for C11 and
 *            C++17 and works for all supported integer types seq_t and idx_t
 * Range:     u8 none, u16 n <= 4, u32 n <= 17, u64 n <= 51, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* -------------- static data, initialized once -------------- */
static int initialized = 0;
static uint8_t  valid_rows[64];      /* binary patterns of a row */
static int      valid_rows_count;    /* number R of valid rows */
static int      next_rows[4096][64]; /* for each state, list of compatible next rows */
static int      next_count[4096];    /* number of entries in next_rows[state] */

/* -------------- helper to build the needed tables once -------------- */
static void init_tables(void)
{
    int bits, j, col;
    /* 1. find all valid row patterns */
    valid_rows_count = 0;
    for (bits = 0; bits < (1 << 6); ++bits) {
        int ok = 1;
        for (j = 0; j <= 3; ++j) {               /* 3 consecutive bits in row */
            int a = (bits >> j)     & 1;
            int b = (bits >> (j+1)) & 1;
            int c = (bits >> (j+2)) & 1;
            if ((a==0 && b==0 && c==1) || (a==1 && b==1 && c==1)) {
                ok = 0;
                break;
            }
        }
        if (ok)
            valid_rows[valid_rows_count++] = (uint8_t)bits;
    }

    int R = valid_rows_count;
    int state_cnt = R * R;                      /* number of (prev2,prev1) states */

    /* 2. build transition lists */
    for (int i = 0; i < state_cnt; ++i) next_count[i] = 0;

    for (int i = 0; i < R; ++i) {
        int prev2 = valid_rows[i];
        for (int j = 0; j < R; ++j) {
            int prev1 = valid_rows[j];
            int state = i * R + j;              /* index of state (prev2,prev1) */
            for (int k = 0; k < R; ++k) {
                int newrow = valid_rows[k];
                int ok = 1;
                for (col = 0; col < 6; ++col) {
                    int a = (prev2 >> col) & 1;
                    int b = (prev1 >> col) & 1;
                    int c = (newrow >> col) & 1;
                    if ((a==0 && b==0 && c==1) || (a==1 && b==1 && c==1)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    next_rows[state][next_count[state]++] = k;
                }
            }
        }
    }

    initialized = 1;
}

/* -------------- main sequence function -------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* offset starts at 1 */
    if (!initialized) init_tables();

    const int R        = valid_rows_count;
    const int state_cnt = R * R;

    /* dynamic arrays, 4096 is max (64*64) */
    static seq_t dp_cur[4096];
    static seq_t dp_next[4096];

    /* zero dp arrays for the first time */
    for (int i = 0; i < state_cnt; ++i) dp_cur[i] = 0;

    /* initialise for the first two rows */
    for (int r1 = 0; r1 < R; ++r1)
        for (int r2 = 0; r2 < R; ++r2)
            dp_cur[r1 * R + r2] = 1;

    /* add rows 3 .. n+2 (total n more rows) */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < state_cnt; ++i) dp_next[i] = 0;
        for (int s = 0; s < state_cnt; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            int count   = next_count[s];
            int prev1   = s % R;                /* second row of the state pair */
            for (int t = 0; t < count; ++t) {
                int newrow    = next_rows[s][t];
                int next_state = prev1 * R + newrow;
                dp_next[next_state] += val;
            }
        }
        /* copy dp_next into dp_cur for next iteration */
        for (int i = 0; i < state_cnt; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all possibilities for the final two rows */
    seq_t ans = 0;
    for (int i = 0; i < state_cnt; ++i) ans += dp_cur[i];
    return ans;
}
