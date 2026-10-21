/*
 * A207811  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Construction: a(n) counts 6×n binary arrays avoiding
   horizontal patterns 000 and 001, and vertical patterns 010 and 101.
   Dynamic‑programming over pairs of consecutive columns is used. */

#include <stdint.h>
#include <string.h>

#define M 26  /* number of 6‑bit columns that avoid 010 and 101 */

static uint8_t col_value[M];          /* the valid columns */
static uint8_t allowed[M][M];         /* allowed[a][b] = 1 if (col[a]|col[b]) == 0x3F */
static int init_done = 0;

/* Check if a 6‑bit column contains the forbidden vertical patterns 010 or 101. */
static int is_valid_column(uint8_t col)
{
    for (int r = 0; r < 4; ++r) {
        int b0 = (col >> r)     & 1;
        int b1 = (col >> (r+1)) & 1;
        int b2 = (col >> (r+2)) & 1;
        if ((b0 == 0 && b1 == 1 && b2 == 0) ||
            (b0 == 1 && b1 == 0 && b2 == 1))
            return 0;
    }
    return 1;
}

/* Initialise the static tables.  Done on the first call. */
static void init_tables(void)
{
    if (init_done) return;
    init_done = 1;

    /* collect valid columns */
    int cnt = 0;
    for (uint8_t col = 0; col < 64; ++col) {
        if (is_valid_column(col)) {
            col_value[cnt++] = col;
        }
    }

    /* build the allowed matrix */
    for (int a = 0; a < M; ++a) {
        for (int b = 0; b < M; ++b) {
            allowed[a][b] = ((col_value[a] | col_value[b]) == 0x3F) ? 1 : 0;
        }
    }
}

seq_t a(idx_t n)
{
    init_tables();

    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)M;

    /* DP arrays: dp[state] where state = a*M + b  (last two columns) */
    static seq_t dp[M*M];
    static seq_t ndp[M*M];

    /* initialise dp for length 2: all pairs are allowed as a single step. */
    for (int i = 0; i < M*M; ++i)
        dp[i] = (seq_t)1;

    /* iterate over columns 3..n */
    for (idx_t step = 3; step <= n; ++step) {
        memset(ndp, 0, sizeof(ndp));
        for (int a = 0; a < M; ++a) {
            for (int b = 0; b < M; ++b) {
                if (!allowed[a][b]) continue;
                seq_t val = dp[a*M + b];
                if (!val) continue;
                for (int c = 0; c < M; ++c) {
                    ndp[b*M + c] = ndp[b*M + c] + val;
                }
            }
        }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* sum all states to get the total number for length n */
    seq_t total = (seq_t)0;
    for (int i = 0; i < M*M; ++i) {
        total = total + dp[i];
    }
    return total;
}
