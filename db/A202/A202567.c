/*
 * A202567  Number of (n+3)X4 binary arrays with no more than two of any
 *          consecutive four bits set in any row or column.
 * https://oeis.org/A202567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+3)×4 binary arrays with no more than two of
 *            any * consecutive four bits set in any row or column. *
 *            Computation by dynamic programming over rows with a state of the
 *            last * three rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint8_t ROWS[11] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 12};
#define MAX_STATE 4096   /* 4 bits per column * 3 rows = 12 bits */
#define MAX_NEXT  11     /* at most 11 admissible next rows */

/* transition tables */
static uint16_t trans[MAX_STATE][MAX_NEXT];
static unsigned char trans_cnt[MAX_STATE];
static int initialized = 0;

static void init_trans(void)
{
    for (int mask = 0; mask < MAX_STATE; ++mask) {
        int r1 =  mask        & 0xF;
        int r2 = (mask >> 4)  & 0xF;
        int r3 = (mask >> 8)  & 0xF;
        unsigned char cnt = 0;

        for (int r = 0; r < 11; ++r) {
            uint8_t next = ROWS[r];
            int ok = 1;
            /* check the window of 4 bits in each column */
            for (int col = 0; col < 4; ++col) {
                int s = ((r1 >> col) & 1) + ((r2 >> col) & 1)
                      + ((r3 >> col) & 1) + ((next >> col) & 1);
                if (s > 2) { ok = 0; break; }
            }
            if (ok) {
                int nmask = (((mask << 4) | next) & 0xFFF);
                trans[mask][cnt++] = (uint16_t)nmask;
            }
        }
        trans_cnt[mask] = cnt;
    }
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init_trans();

    const idx_t N = n + 3;            /* number of rows */
    /* dp arrays */
    seq_t dp[MAX_STATE];
    seq_t dp_next[MAX_STATE];

    /* clear dp */
    for (int i = 0; i < MAX_STATE; ++i) dp[i] = 0;

    /* initial first 3 rows */
    for (int i = 0; i < 11; ++i) {
        uint8_t r1 = ROWS[i];
        for (int j = 0; j < 11; ++j) {
            uint8_t r2 = ROWS[j];
            for (int k = 0; k < 11; ++k) {
                uint8_t r3 = ROWS[k];
                int mask = r1 | (r2 << 4) | (r3 << 8);
                dp[mask] = 1;
            }
        }
    }

    /* remaining rows */
    const idx_t steps = (N > 3) ? (N - 3) : 0;
    for (idx_t step = 0; step < steps; ++step) {
        /* clear dp_next */
        for (int i = 0; i < MAX_STATE; ++i) dp_next[i] = 0;

        for (int mask = 0; mask < MAX_STATE; ++mask) {
            seq_t val = dp[mask];
            if (val == 0) continue;
            unsigned char cnt = trans_cnt[mask];
            uint16_t *tbl = trans[mask];
            for (unsigned char t = 0; t < cnt; ++t) {
                dp_next[tbl[t]] += val;
            }
        }
        /* move to next iteration */
        for (int i = 0; i < MAX_STATE; ++i) dp[i] = dp_next[i];
    }

    /* sum all final states */
    seq_t total = 0;
    for (int mask = 0; mask < MAX_STATE; ++mask) total += dp[mask];
    return total;
}
