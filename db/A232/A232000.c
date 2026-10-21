/*
 * A232000  Number of (3+1)X(n+1) 0..1 arrays with no element having a strict
 *          majority of its horizontal and antidiagonal neighbors equal to
 *          one.
 * https://oeis.org/A232000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4 × (n + 1) binary arrays with no element
 *            having a * strict majority of its horizontal and antidiagonal
 *            neighbors equal to 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   bit_col, valid_first_pair, valid_last_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bit_col.h"
#include "../utils/valid_first_pair.h"
#include "../utils/valid_last_col.h"

/* Check validity of the middle column (curr) given the previous (prevprev)
 * and next (next) columns.  All four neighbors exist. */
static inline bool valid_middle(int prevprev, int curr, int next)
{
    (void)curr;  /* curr itself is not used in the check */
    for (int r = 0; r < 4; ++r) {
        int k   = 2;                      /* left (prevprev) and right (next) */
        int cnt = bit_col(prevprev, r) + bit_col(next, r);
        if (r > 0) {                     /* NE from next */
            ++k;
            cnt += bit_col(next, r-1);
        }
        if (r < 3) {                     /* SW from prevprev */
            ++k;
            cnt += bit_col(prevprev, r+1);
        }
        if (cnt > (k >> 1)) return false;
    }
    return true;
}

/* Tables for fast DP */
static bool tables_initialized = false;
static unsigned char init_valid[256];
static unsigned char final_valid[256];
static unsigned char trans_table[256][16];

static void init_tables(void)
{
    for (int c0 = 0; c0 < 16; ++c0) {
        for (int c1 = 0; c1 < 16; ++c1) {
            init_valid[(c0 << 4) | c1] = valid_first_pair(c0, c1);
        }
    }
    for (int prev = 0; prev < 16; ++prev) {
        for (int cur = 0; cur < 16; ++cur) {
            final_valid[(prev << 4) | cur] = valid_last_col(prev, cur);
        }
    }
    for (int s = 0; s < 256; ++s) {
        int prevprev = s >> 4;
        int prev     = s & 0xF;
        for (int nxt = 0; nxt < 16; ++nxt) {
            trans_table[s][nxt] = valid_middle(prevprev, prev, nxt);
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!tables_initialized) {
        init_tables();
        tables_initialized = true;
    }

    idx_t width = n + (idx_t)1;          /* number of columns (n+1) */
    if (width == 1) {                    /* single column */
        return (seq_t)16;                /* 2^4 possible assignments */
    }

    seq_t dp_cur[256]    = {0};
    seq_t dp_next[256];
    /* Initialize DP with valid first pairs */
    for (int c0 = 0; c0 < 16; ++c0) {
        for (int c1 = 0; c1 < 16; ++c1) {
            int state = (c0 << 4) | c1;
            if (init_valid[state]) {
                dp_cur[state] = (seq_t)1;
            }
        }
    }

    if (width > 2) {
        for (idx_t col = 2; col < width; ++col) {
            /* Reset dp_next */
            for (int i = 0; i < 256; ++i) dp_next[i] = (seq_t)0;
            for (int s = 0; s < 256; ++s) {
                seq_t cnt = dp_cur[s];
                if (!cnt) continue;
                int prevprev = s >> 4;
                int prev     = s & 0xF;
                for (int nxt = 0; nxt < 16; ++nxt) {
                    if (trans_table[s][nxt]) {
                        int nstate = (prev << 4) | nxt;
                        dp_next[nstate] = dp_next[nstate] + cnt;
                    }
                }
            }
            /* Move dp_next to dp_cur for next iteration */
            for (int i = 0; i < 256; ++i) dp_cur[i] = dp_next[i];
        }
    }

    /* Sum over final states where the last column satisfies constraints */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 256; ++s) {
        seq_t cnt = dp_cur[s];
        if (!cnt) continue;
        if (final_valid[s]) {
            result = result + cnt;
        }
    }
    return result;
}
