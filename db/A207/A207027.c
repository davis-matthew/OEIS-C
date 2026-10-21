/*
 * A207027  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 25, u64 n <= 61, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of 4×n binary arrays avoiding horizontal patterns 000,011
   and vertical patterns 001,101 by dynamic programming over column states. */

#include <stdbool.h>

static const int VALID_COLS[8] = {0, 4, 6, 7, 8, 12, 14, 15}; /* 4‑bit column patterns */
static int TRANS[256][8];
static bool TRANS_INI = false;

static void init_transitions(void) {
    if (TRANS_INI) return;
    for (int s = 0; s < 256; ++s) {
        for (int p = 0; p < 8; ++p) {
            bool ok = true;
            int new_state = 0;
            for (int r = 0; r < 4; ++r) {
                int pair = (s >> (r * 2)) & 3;           /* bits from two previous columns */
                int a = (pair >> 1) & 1;                 /* bit t-2 */
                int b = pair & 1;                        /* bit t-1 */
                int c = (VALID_COLS[p] >> (3 - r)) & 1;  /* new bit */

                if (a == 0 && b == 0 && c == 0) { ok = false; break; } /* 000 forbidden */
                if (a == 0 && b == 1 && c == 1) { ok = false; break; } /* 011 forbidden */

                int new_pair = (b << 1) | c;            /* bits t-1 and t */
                new_state |= (new_pair << (r * 2));
            }
            TRANS[s][p] = ok ? new_state : -1;
        }
    }
    TRANS_INI = true;
}

seq_t a(idx_t n) {
    init_transitions();

    if (n == 1) return 8;
    if (n == 2) return 64;

    seq_t dp[256] = {0};
    seq_t next[256];

    /* initial states after first two columns (no horizontal restrictions yet) */
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int state = 0;
            for (int r = 0; r < 4; ++r) {
                int b1 = (VALID_COLS[i] >> (3 - r)) & 1;
                int b2 = (VALID_COLS[j] >> (3 - r)) & 1;
                int pair = (b1 << 1) | b2;
                state |= (pair << (r * 2));
            }
            dp[state] += 1;
        }
    }

    /* process remaining columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        for (int s = 0; s < 256; ++s) next[s] = 0;
        for (int s = 0; s < 256; ++s) {
            seq_t cnt = dp[s];
            if (!cnt) continue;
            for (int pi = 0; pi < 8; ++pi) {
                int ns = TRANS[s][pi];
                if (ns >= 0) next[ns] += cnt;
            }
        }
        for (int s = 0; s < 256; ++s) dp[s] = next[s];
    }

    /* total count */
    seq_t total = 0;
    for (int s = 0; s < 256; ++s) total += dp[s];
    return total;
}
