/*
 * A207931  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary matrices avoiding the * horizontal
 *            patterns 000 and 010 in every row and the vertical * patterns
 *            000 and 111 in every column. * * For n = 1: a(1) = 16 * For n =
 *            2: a(2) = 256 * For n ≥ 3: computed by a transfer‑matrix DP over
 *            column states
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_COLS 32      /* maximum number of column patterns (2^5) */

static bool   init_done = false;
static idx_t  num_valid = 0;                /* number of vertical‑valid columns   */
static idx_t  cols[MAX_COLS];               /* bit patterns of valid columns      */
static bool   allowed[MAX_COLS][MAX_COLS][MAX_COLS]; /* horizontal validity */

static seq_t dp[MAX_COLS][MAX_COLS];
static seq_t ndp[MAX_COLS][MAX_COLS];

/* Test whether a 5‑bit column satisfies the vertical constraints
 * (no three consecutive 0s or 1s). */
static bool vertical_ok (idx_t bits)
{
    for (int r = 0; r <= 2; ++r) {
        int b0 = (bits >> r)     & 1;
        int b1 = (bits >> (r+1)) & 1;
        int b2 = (bits >> (r+2)) & 1;
        if (b0 == 0 && b1 == 0 && b2 == 0) return false;
        if (b0 == 1 && b1 == 1 && b2 == 1) return false;
    }
    return true;
}

/* Precompute column patterns and transfer matrix.  This runs only once. */
static void init(void)
{
    /* find all vertical‑valid columns */
    num_valid = 0;
    for (idx_t bits = 0; bits < 32; ++bits) {
        if (vertical_ok(bits)) {
            cols[num_valid++] = bits;
        }
    }

    /* compute horizontal transfer matrix */
    for (int a = 0; a < (int)num_valid; ++a) {
        for (int b = 0; b < (int)num_valid; ++b) {
            for (int c = 0; c < (int)num_valid; ++c) {
                bool ok = true;
                /* for every row, the three bits must not be 000 or 010 */
                for (int r = 0; r < 5; ++r) {
                    int b2 = (cols[a] >> r) & 1;   /* column a */
                    int b1 = (cols[b] >> r) & 1;   /* column b */
                    int b0 = (cols[c] >> r) & 1;   /* column c */
                    if (b2 == 0 && b1 == 0 && b0 == 0) { ok = false; break; }
                    if (b2 == 0 && b1 == 1 && b0 == 0) { ok = false; break; }
                }
                allowed[a][b][c] = ok;
            }
        }
    }

    init_done = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset undefined, but harmless */

    if (!init_done) init();

    /* n = 1 and n = 2 are trivial */
    if (n == 1) return (seq_t)num_valid;
    if (n == 2) return (seq_t)num_valid * (seq_t)num_valid;

    /* DP arrays: cur_dp holds counts for the current number of columns */
    seq_t (*cur_dp)[MAX_COLS] = dp;
    seq_t (*next_dp)[MAX_COLS] = ndp;

    /* initialise for length = 2: every pair of valid columns is possible */
    for (int i = 0; i < (int)num_valid; ++i)
        for (int j = 0; j < (int)num_valid; ++j)
            cur_dp[i][j] = (seq_t)1;

    /* iterate over remaining columns (from 3 up to n) */
    for (idx_t col = 3; col <= n; ++col) {
        /* clear next_dp */
        for (int i = 0; i < (int)num_valid; ++i)
            for (int j = 0; j < (int)num_valid; ++j)
                next_dp[i][j] = (seq_t)0;

        /* transition */
        for (int p2 = 0; p2 < (int)num_valid; ++p2) {
            for (int p1 = 0; p1 < (int)num_valid; ++p1) {
                seq_t val = cur_dp[p1][p2];
                if (val == (seq_t)0) continue;
                for (int cur = 0; cur < (int)num_valid; ++cur) {
                    if (allowed[p2][p1][cur]) {
                        next_dp[cur][p1] += val;
                    }
                }
            }
        }
        /* swap pointers for next iteration */
        seq_t (*tmp)[MAX_COLS] = cur_dp;
        cur_dp = next_dp;
        next_dp = tmp;
    }

    /* sum over all ending states */
    seq_t total = (seq_t)0;
    for (int i = 0; i < (int)num_valid; ++i)
        for (int j = 0; j < (int)num_valid; ++j)
            total += cur_dp[i][j];

    return total;
}
