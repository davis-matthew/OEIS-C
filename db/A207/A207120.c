/*
 * A207120  Number of nX5 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): Number of n×5 0..1 arrays avoiding
   horizontally 0 0 1 and 0 1 0, and vertically 0 0 1 and 0 1 1.
   Computed by dynamic programming over rows with state (previous two rows).
   The implementation is independent of the specific size of seq_t. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool initialized = false;

/* Pre‑computed data for 5‑bit rows (0..31) */
static bool            row_valid[32];
static uint8_t         trans_cnt[32][32];
static unsigned short  trans_from[32][32][32];   /* up to 32 successors per (a,b) */
static unsigned int    valid_rows;               /* number of valid rows */

/* Dynamic programming tables: dp[cur][a][b] = number of length‑k sequences
   ending with rows a (second‑last) and b (last). */
static seq_t dp[2][32][32];

/* One‑time initialization of validity and transition tables. */
static void init_once(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Identify rows that satisfy horizontal constraints. */
    for (unsigned int r = 0; r < 32; ++r) {
        bool ok = true;
        for (unsigned int i = 0; i + 2 < 5; ++i) {
            unsigned int seg = (r >> i) & 7;          /* 3 consecutive bits */
            if (seg == 0x01 || seg == 0x02) {         /* 001 or 010 */
                ok = false;
                break;
            }
        }
        row_valid[r] = ok;
        if (ok)
            ++valid_rows;
    }

    /* Build transition tables for pairs of consecutive rows. */
    for (unsigned int a = 0; a < 32; ++a) if (row_valid[a]) {
        for (unsigned int b = 0; b < 32; ++b) if (row_valid[b]) {
            unsigned int cnt = 0;
            for (unsigned int c = 0; c < 32; ++c) if (row_valid[c]) {
                bool ok = true;
                for (unsigned int col = 0; col < 5; ++col) {
                    unsigned int abit  = (a >> col) & 1;
                    unsigned int bbit  = (b >> col) & 1;
                    unsigned int cbit  = (c >> col) & 1;
                    if ((abit == 0 && bbit == 0 && cbit == 1) ||
                        (abit == 0 && bbit == 1 && cbit == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    trans_from[a][b][cnt++] = (unsigned short)c;
                }
            }
            trans_cnt[a][b] = (uint8_t)cnt;
        }
    }
}

/* Main entry for the sequence. */
seq_t a(idx_t n)
{
    init_once();

    if (n == 0)
        return 0;                 /* undefined for n < 1 */

    /* Convert n to unsigned for loop bounds (n <= 10 000 for performance). */
    unsigned int nu = (unsigned int)n;

    /* Single row: just count valid rows. */
    if (nu == 1)
        return (seq_t)valid_rows;

    /* Two rows: every pair of valid rows is allowed. */
    if (nu == 2)
        return (seq_t)valid_rows * (seq_t)valid_rows;

    /* DP for n >= 3 */
    memset(dp, 0, sizeof(dp));

    /* Base state for length 2: all valid (a,b) pairs have count 1. */
    int cur = 0, nxt = 1;
    for (unsigned int a = 0; a < 32; ++a) if (row_valid[a]) {
        for (unsigned int b = 0; b < 32; ++b) if (row_valid[b]) {
            dp[cur][a][b] = (seq_t)1;
        }
    }

    /* Perform transitions for each additional row. */
    for (unsigned int step = 0; step < nu - 2; ++step) {
        memset(dp[nxt], 0, sizeof(dp[nxt]));
        for (unsigned int a = 0; a < 32; ++a) {
            for (unsigned int b = 0; b < 32; ++b) {
                seq_t cnt = dp[cur][a][b];
                if (cnt == 0)
                    continue;
                unsigned int tcnt = trans_cnt[a][b];
                for (unsigned int t = 0; t < tcnt; ++t) {
                    unsigned short c = trans_from[a][b][t];
                    dp[nxt][b][c] += cnt;
                }
            }
        }
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* Sum all sequences of length n. */
    seq_t result = 0;
    for (unsigned int a = 0; a < 32; ++a) {
        for (unsigned int b = 0; b < 32; ++b) {
            result += dp[cur][a][b];
        }
    }
    return result;
}
