/*
 * A207948  Number of nX7 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×7 0..1 arrays avoiding horizontally 0 0 1 and 0 1 1
 *            and vertically 0 1 0 and 1 0 1. The code implements a
 *            linear‑time dynamic‑programming algorithm. States are pairs of
 *            admissible 7‑bit rows (400 states). For each state the list of
 *            admissible next rows is pre‑computed once. The recurr...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Return a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------
       1. Build the admissible 7‑bit row patterns (20 total).
       ------------------------------------------------------------ */
    static bool built = false;
    static int patterns[20];        /* indices of the 128 possible rows */
    static int bit[20][7];          /* each pattern's bits: bit[row][col] */
    static int next_k[400][20];     /* admissible next rows for each state */
    static int cnt_next[400];       /* number of admissible next rows per state */
    const int PATCNT = 20;          /* we know there are exactly 20 patterns */

    if (!built) {
        int patcnt = 0;

        /* Generate all 7‑bit rows that avoid 0 0 1 and 0 1 1 horizontally */
        for (int p = 0; p < 128; ++p) {
            bool ok = true;
            for (int j = 0; j <= 4; ++j) {     /* windows of length 3 */
                int b0 = (p >> j)       & 1;
                int b1 = (p >> (j + 1)) & 1;
                int b2 = (p >> (j + 2)) & 1;
                if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                    (b0 == 0 && b1 == 1 && b2 == 1)) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;
            patterns[patcnt] = p;
            for (int col = 0; col < 7; ++col)
                bit[patcnt][col] = (p >> col) & 1;
            ++patcnt;
        }

        /* Build transitions: for each pair (prev2,prev1) enumerate admissible
           next rows that keep the vertical restrictions (0 1 0, 1 0 1). */
        for (int i = 0; i < PATCNT; ++i) {
            for (int j = 0; j < PATCNT; ++j) {
                int src = i * PATCNT + j;
                int c = 0;
                for (int k = 0; k < PATCNT; ++k) {
                    bool ok = true;
                    for (int col = 0; col < 7; ++col) {
                        int a = bit[i][col];
                        int b = bit[j][col];
                        int d = bit[k][col];
                        if ((a == 0 && b == 1 && d == 0) ||
                            (a == 1 && b == 0 && d == 1)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        next_k[src][c] = k;
                        ++c;
                    }
                }
                cnt_next[src] = c;
            }
        }
        built = true;
    }

    /* ------------------------------------------------------------
       2. Handle the base cases n = 1, 2.
       ------------------------------------------------------------ */
    if (n == 0) {
        return 0; /* defined behaviour for n=0 is unused */
    }
    if (n == 1) {
        return (seq_t)PATCNT;                    /* 20 */
    }
    if (n == 2) {
        return (seq_t)PATCNT * (seq_t)PATCNT;     /* 400 */
    }

    /* ------------------------------------------------------------
       3. Dynamic programming for rows 3 … n.
       ------------------------------------------------------------ */
    const int STATES = PATCNT * PATCNT;          /* 400 */

    seq_t *dp_cur  = (seq_t *)calloc(STATES, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(STATES, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        free(dp_cur);
        free(dp_next);
        return 0; /* out of memory */
    }

    /* Start with all two‑row prefixes: each pair of patterns is allowed. */
    for (int i = 0; i < PATCNT; ++i) {
        for (int j = 0; j < PATCNT; ++j) {
            int idx = i * PATCNT + j;
            dp_cur[idx] = 1;
        }
    }

    /* Iterate over remaining rows.  We already placed 2 rows, so
       we need to add n-2 more rows. */
    for (idx_t step = 2; step < n; ++step) {
        /* zero dp_next */
        for (int s = 0; s < STATES; ++s) dp_next[s] = 0;

        /* For each current state, transition to all admissible next rows */
        for (int src = 0; src < STATES; ++src) {
            seq_t val = dp_cur[src];
            if (!val) continue;
            int allowed = cnt_next[src];
            int prev1 = src % PATCNT;          /* j in (i,j) pair */
            for (int t = 0; t < allowed; ++t) {
                int k = next_k[src][t];
                int dst = prev1 * PATCNT + k; /* new state (prev1, k) */
                dp_next[dst] += val;
            }
        }

        /* swap dp arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* ------------------------------------------------------------
       4. Sum all final states to get total count.
       ------------------------------------------------------------ */
    seq_t total = 0;
    for (int s = 0; s < STATES; ++s) {
        total += dp_cur[s];
    }

    free(dp_cur);
    free(dp_next);
    return total;
}
